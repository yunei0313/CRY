﻿using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;


namespace Editor_WPF.Utilities;


//---------------------------------------------------------------------------------------------------------------------
/// Serializer
//---------------------------------------------------------------------------------------------------------------------
public static class Serializer
{
    //-----------------------------------------------------------------------------------------------------------------
    /// ToFile
    //-----------------------------------------------------------------------------------------------------------------
    public static void ToFile< T >( T instance, string path )
    {
        try
        {
            using FileStream fs = new FileStream( path, FileMode.Create );
            DataContractSerializer serializer = new DataContractSerializer( typeof( T ) );
            
            serializer.WriteObject( fs, instance );
        }
        catch ( Exception e )
        {
            Debug.WriteLine( e.Message );
            
            Logger.Log( MessageType.Error, "Failed to serialize {instance} to {path}" );

            throw;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// FromFile
    //-----------------------------------------------------------------------------------------------------------------
    internal static T FromFile< T >( string path )
    {
        try
        {
            using FileStream fs = new FileStream( path, FileMode.Open );
            DataContractSerializer serializer = new DataContractSerializer( typeof( T ) );
            
            T instance = (T)serializer.ReadObject( fs )!;
            
            return instance;
        }
        catch ( Exception e )
        {
            Debug.WriteLine( e.Message );

            Logger.Log( MessageType.Error, "Failed to deserialize {path}" );

            throw;
        }
    }
}
