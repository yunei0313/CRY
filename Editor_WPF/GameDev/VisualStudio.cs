﻿using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using Editor_WPF.GameProject;
using Editor_WPF.Utilities;
using EnvDTE80;
using Path = System.IO.Path;



namespace Editor_WPF.GameDev;


internal abstract class EnvDTEConstants
{
    public const string vsViewKindTextView = "{7651A703-06E5-11D1-8EBD-00A0C90F26EA}";
}


//---------------------------------------------------------------------------------------------------------------------
/// VisualStudio
//---------------------------------------------------------------------------------------------------------------------
public static class VisualStudio
{
    private static DTE2? _vsInstance;
    private const string ProgramId = "VisualStudio.DTE.17.0";

    public static bool BuildSucceeded { get; private set; } = true;
    public static bool BuildDone { get; private set; } = true;


    [DllImport( "ole32.dll" )]
    private static extern int CreateBindCtx( uint reserved, out IBindCtx? ppBindCtx );

    [DllImport( "ole32.dll" )]
    private static extern int GetRunningObjectTable( uint reserved, out IRunningObjectTable? ppRunningObjectTable );

    //-----------------------------------------------------------------------------------------------------------------
    /// OpenVisualStudio
    //-----------------------------------------------------------------------------------------------------------------
    public static void OpenVisualStudio( string solutionPath )
    {
        IRunningObjectTable? runningObjectTable = null;
        IEnumMoniker? monikerTable = null;
        IBindCtx? bindCtx = null;

        try
        {
            if ( _vsInstance == null )
            {
                int result = GetRunningObjectTable( 0, out runningObjectTable );
                if ( result < 0 || runningObjectTable == null )
                {
                    throw new COMException( $"GetRunningObjectTable() returned HRESULT : {result:X8}" );
                }

                runningObjectTable.EnumRunning( out monikerTable );
                monikerTable.Reset();

                result = CreateBindCtx( 0, out bindCtx );
                if ( result < 0 || bindCtx == null )
                {
                    throw new COMException( $"CreateBindCtx() returned HRESULT : {result:X8}" );
                }

                IMoniker[] currentMoniker = new IMoniker[ 1 ];
                bool isOpen = false;
                while ( !isOpen && monikerTable.Next( 1, currentMoniker, IntPtr.Zero ) == 0 )
                {
                    currentMoniker[ 0 ].GetDisplayName( bindCtx, null, out string name );

                    if ( name.Contains( ProgramId ) )
                    {
                        result = runningObjectTable.GetObject( currentMoniker[ 0 ], out object obj );
                        if ( result < 0 || obj == null )
                        {
                            throw new COMException( $"Running object table's GetObject() returned HRESULT : {result:X8}" );
                        }

                        DTE2? dte = obj as DTE2;
                        string? solutionName = dte?.Solution.FullName;
                        if ( solutionName == solutionPath )
                        {
                            _vsInstance = dte;
                            break;
                        }
                    }
                }

                if ( _vsInstance == null )
                {
                    Type? type = Type.GetTypeFromProgID( ProgramId );
                    if ( type != null )
                    {
                        _vsInstance = Activator.CreateInstance( type ) as DTE2;
                    }
                }
            }
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( ex.Message );
            Logger.Log( MessageType.Error, "Failed to open visual studio." );
        }
        finally
        {
            if ( monikerTable != null ) Marshal.ReleaseComObject( monikerTable );
            if ( runningObjectTable != null ) Marshal.ReleaseComObject( runningObjectTable );
            if ( bindCtx != null ) Marshal.ReleaseComObject( bindCtx );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// CloseVisualStudio
    //-----------------------------------------------------------------------------------------------------------------
    public static void CloseVisualStudio()
    {
        if ( _vsInstance?.Solution.IsOpen == true )
        {
            _vsInstance.ExecuteCommand( "File.SaveAll" );
            _vsInstance.Solution.Close();
        }

        _vsInstance?.Quit();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// AddFilesToSolution
    //-----------------------------------------------------------------------------------------------------------------
    public static bool AddFilesToSolution( string solution, string projectName, string[] files )
    {
        OpenVisualStudio( solution );

        try
        {
            if ( _vsInstance != null )
            {
                if ( !_vsInstance.Solution.IsOpen ) _vsInstance.Solution.Open( solution );
                else _vsInstance.ExecuteCommand( "File.SaveAll" );

                foreach ( EnvDTE.Project project in _vsInstance.Solution.Projects )
                {
                    if ( project.UniqueName.Contains( projectName ) )
                    {
                        foreach ( var file in files )
                        {
                            project.ProjectItems.AddFromFile( file );
                        }
                    }
                }

                string? cpp = files.FirstOrDefault( x => Path.GetExtension( x ) == ".cpp" );

                if ( !string.IsNullOrEmpty( cpp ) )
                {
                    _vsInstance.ItemOperations.OpenFile( cpp, EnvDTEConstants.vsViewKindTextView ).Visible = true;
                }

                _vsInstance.MainWindow.Activate();
                _vsInstance.MainWindow.Visible = true;

            }
        }
        catch ( Exception exception )
        {
            Debug.WriteLine( exception.Message );
            Debug.WriteLine( "faiuled to add files to visual studio project" );

            return false;
        }

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// BuildSolution
    //-----------------------------------------------------------------------------------------------------------------
    internal static void BuildSolution( Project project, string configName, bool showWindow = true )
    {
        if ( IsDebugging() )
        {
            Logger.Log( MessageType.Error, "Visual studio is currenty running a process." );
            return;
        }

        OpenVisualStudio( project.Solution );
        BuildDone = BuildSucceeded = false;

        for ( int i = 0; i < 3; ++i )
        {
            try
            {
                if ( _vsInstance != null && !_vsInstance.Solution.IsOpen )
                {
                    _vsInstance.Solution.Open( project.Solution );
                }

                if ( _vsInstance != null )
                {
                    _vsInstance.MainWindow.Visible = showWindow;

                    _vsInstance.Events.BuildEvents.OnBuildProjConfigBegin += OnBuildSolutionBegin;
                    _vsInstance.Events.BuildEvents.OnBuildProjConfigDone += OnBuildSolutionDone;

                    _vsInstance.Solution.SolutionBuild.SolutionConfigurations.Item( configName ).Activate();
                    _vsInstance.ExecuteCommand( "Build.BuildSolution" );
                }
            }
            catch ( Exception exception )
            {
                Debug.WriteLine( exception.Message );
                Debug.WriteLine( $"Attempt {i}: failed to build {project.Name}" );

                System.Threading.Thread.Sleep( 1000 );
            }
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// IsDebugging
    //-----------------------------------------------------------------------------------------------------------------
    public static bool IsDebugging()
    {
        bool result = false;

        try
        {
            result = _vsInstance != null && ( _vsInstance.Debugger.CurrentProgram != null || _vsInstance.Debugger.CurrentMode == EnvDTE.dbgDebugMode.dbgRunMode );
        }
        catch ( Exception exception )
        {
            Debug.Write( exception.Message );

            if ( !result )
            {
                System.Threading.Thread.Sleep( 1000 );
            }
        }

        return result;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnBuildSolutionDone
    //-----------------------------------------------------------------------------------------------------------------
    private static void OnBuildSolutionDone( string project, string projectconfig, string platform, string solutionconfig, bool success )
    {
        if ( BuildDone ) return;

        if ( success )
        {
            Logger.Log( MessageType.Info, $"Building {projectconfig} configuration succeeded" );
        }
        else
        {
            Logger.Log( MessageType.Error, $"Building {projectconfig} configuration failed" );
        }

        BuildDone = true;
        BuildSucceeded = success;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnBuildSolutionBegin
    //-----------------------------------------------------------------------------------------------------------------
    private static void OnBuildSolutionBegin( string project, string projectconfig, string platform, string solutionconfig )
    {
        Logger.Log( MessageType.Info, $"Building {project}, {projectconfig}, {platform}, {solutionconfig}" );
    }
}