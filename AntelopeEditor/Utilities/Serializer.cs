using System;
using System.IO;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.Threading.Tasks;
using AntelopeEditor.Utilities;

namespace AntelopeEditor
{
    public static class Serializer
    {
        public static void ToFile<T>(T instance, string path)
        {
            try
            {
                using var fs = new FileStream(path, FileMode.Create);
                var serialiser = new DataContractSerializer(typeof(T));
                serialiser.WriteObject(fs, instance);
            }
            catch (Exception ex)
            {
                Logger.Log(MessageType.Error, $"Failed to serialize {instance} to {path}");
                Debug.WriteLine(ex);
            }
        }

        public static T FromFile<T>(string path) 
        {
            try
            {
                using var fs = new FileStream(path, FileMode.Open);
                var serialiser = new DataContractSerializer(typeof(T));
                T instance = (T)serialiser.ReadObject(fs);
                return instance;
            }
            catch (Exception ex)
            {
                Logger.Log(MessageType.Error, $"Failed to serialize {path}");
                Debug.WriteLine(ex);
                throw;
            }
        }
    }
}
