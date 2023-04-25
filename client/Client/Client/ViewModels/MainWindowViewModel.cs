using System;
using System.Collections.ObjectModel;
using System.Data.SqlTypes;
using System.IO;
using System.Net;
using System.Reactive;
using Avalonia.Controls;
using Client.TcpClient;
using ReactiveUI;
using ReactiveUI.Fody.Helpers;

namespace Client.ViewModels
{
    public class MainWindowViewModel : ViewModelBase
    {
        [Reactive] public string IpAddress { get; set; } = "127.0.0.1";
        [Reactive] public int Port { get; set; } = 6720;
        [Reactive] public string MessageFromServer { get; set; }
        [Reactive] public int SelectedIndexListBox { get; set; }
        [Reactive] public int SelectedIndexComboBox { get; set; }
        public ObservableCollection<string> ListBoxItems { get; set; }
        public ObservableCollection<string> ComboBoxItems { get; set; }
        private string _path;
        public ReactiveCommand<Unit, Unit> ConnectToServer { get; }
        public ReactiveCommand<Unit, Unit> DisconnectFromServer { get; }
        public ReactiveCommand<Unit, Unit> SendMessageToServer { get; }
        public ReactiveCommand<Unit, Unit> ShutOffServer { get; }

        private TcpClient.TcpClient _client;

        public MainWindowViewModel()
        {
            var directoryParse = new DirectoryParse();
            
            ListBoxItems = new ObservableCollection<string>();
            
            ComboBoxItems = new ObservableCollection<string>();
            
            string rootDirectory = "/home/darling/My project";
            string[] files = Directory.GetFiles(rootDirectory);
            string[] directory = Directory.GetDirectories(rootDirectory);
            
            foreach (var file in files)
            {
                ListBoxItems.Add(file);
            }
            
            foreach (var t in directory)
            {
                ListBoxItems.Add(t);
            }

            ConnectToServer = ReactiveCommand.Create(() =>
            {
                if (IpParse())
                {
                    _client = new TcpClient.TcpClient(IpAddress, Port);

                    if (_client.IsConnected) MessageFromServer = "Подключение установлено успешно.";
                    else MessageFromServer = "Подключение не удалось...";
                }
                else MessageFromServer = "Неправильно введён ip.";
            });
            DisconnectFromServer = ReactiveCommand.Create(() =>
            {
                if (_client != null && _client.IsConnected) _client.CloseConnection();
                else MessageFromServer = "Подключение с сервером ещё не установлено.";
            });
            SendMessageToServer = ReactiveCommand.Create(() =>
            {
                //_path = ListBoxItems[SelectedIndexListBox];

                if (_client != null && _client.IsConnected && string.IsNullOrEmpty(_path))
                {
                    var buf = directoryParse.StringParse(_client.SendMessageToServer(_path));
                    
                    ReplaceItems(buf);
                }
                else if (_client == null || _client.IsConnected == false)
                    MessageFromServer = "Подключение с сервером ещё не установлено.";
                else MessageFromServer = "Не выбран путь.";
            });
            ShutOffServer = ReactiveCommand.Create(() =>
            {
                if (_client != null && _client.IsConnected) _client.CloseServer();
                else MessageFromServer = "Подключение с сервером ещё не установлено.";
            });
        }

        private bool IpParse()
        {
            IPAddress ip;
            
            return IPAddress.TryParse(IpAddress, out ip);
        }

        private void ReplaceItems(ObservableCollection<string> buff)
        {
            for (int i = 0; i < buff.Count; i++)
            {
                if (i < ListBoxItems.Count) ListBoxItems[i] = buff[i];
                else ListBoxItems.Add(buff[i]);
            }

            if (buff.Count < ListBoxItems.Count)
            {
                for (int i = ListBoxItems.Count - 1; i > buff.Count - 1; i++)
                {
                    ListBoxItems.RemoveAt(i);
                }
            }
        }
    }
}