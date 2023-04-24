using System;
using System.Collections.ObjectModel;
using System.Data.SqlTypes;
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
        public ObservableCollection<string> ListBoxItems { get; set; }
        private string _path;
        public ReactiveCommand<Unit, Unit> ConnectToServer { get; }
        public ReactiveCommand<Unit, Unit> DisconnectFromServer { get; }
        public ReactiveCommand<Unit, Unit> SendMessageToServer { get; }
        public ReactiveCommand<Unit, Unit> ShutOffServer { get; }
        private readonly DirectoryParse _directoryParse;

        private TcpClient.TcpClient _client;

        public MainWindowViewModel()
        {
            _directoryParse = new DirectoryParse();
            ListBoxItems = new ObservableCollection<string>();
            ListBoxItems.Add("1");
            ListBoxItems.Add("2");
            ListBoxItems.Add("3");
            ListBoxItems.Add("4");
            
            ConnectToServer = ReactiveCommand.Create(() =>
            {
                if (IpParse())
                {
                    _client = new TcpClient.TcpClient(IpAddress, Port);

                    if (_client.IsConnected) MessageFromServer = "Подключение установлено успешно.";
                    else MessageFromServer = "Подключение не удалось";
                }
                else MessageFromServer = "Неправильно введён ip";
            });
            DisconnectFromServer = ReactiveCommand.Create(() =>
            {
                if (_client != null && _client.IsConnected) _client.CloseConnection();
                else MessageFromServer = "Подключение с сервером ещё не установлено.";
            });
            SendMessageToServer = ReactiveCommand.Create(() =>
            {
                _path = ListBoxItems[SelectedIndexListBox];

                if (_client != null && _client.IsConnected && string.IsNullOrEmpty(_path))
                {
                    var buf = _directoryParse.StringParse(_client.SendMessageToServer(_path));
                    
                    ReplaceItems(buf);
                }
                else if (_client == null)
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
            int count = 0;
            int chunkCount = 0;
            var buff = "";
            
            for (int i = 0; i < IpAddress.Length; i++)
            {
                if (IpAddress[i] == '.')
                {
                    if (count > 3) return false;
                    if (Convert.ToInt32(buff) > 255) return false;
                    
                    count = 0;
                    buff = "";
                    chunkCount += 1;

                    if (chunkCount > 4) return false;
                }
                else
                {
                    count += 1;
                    buff += IpAddress[i].ToString();

                    if (count > 3) return false;
                }
            }

            chunkCount += 1;
            
            if (Convert.ToInt32(buff) > 255 || chunkCount > 4) return false;
            
            return true;
        }

        private void ReplaceItems(ObservableCollection<string> buff)
        {
            buff.Add("123");
            buff.Add("1234");
            buff.Add("12345");
                    
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