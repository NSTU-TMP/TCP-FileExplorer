using System;using System.Collections.Generic;using System.Collections.ObjectModel;using System.Linq;using Avalonia.Controls;using DynamicData;namespace Client.TcpClient;public class Parse{    public Parse() { }    public static List<string> StringParse(string concatStr)    {        var strings = concatStr.Split('\n');        var _listBoxItems = new List<string>();                foreach (var str in strings)        {            _listBoxItems.Add(str);        }        return _listBoxItems;    }    public static string CutThePath(string path)    {        var bufStr = "";                for (int i = path.Length - 1; i > 0; i--)        {            var buf = i;            if (path[i] == '/')            {                bufStr = path[0..buf];                break;            }        }                if (path.Length > 0 && bufStr.Length == 0) bufStr += "/";                return bufStr;    }}