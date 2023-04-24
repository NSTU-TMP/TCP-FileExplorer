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
        [Reactive] public string ipAddress { get; set; } = "127.0.0.1";
        [Reactive] public int port { get; set; } = 1024;
        [Reactive] public string messageFromServer { get; set; }
        [Reactive] public int selectedIndexListBox { get; set; }
        public ObservableCollection<string> listBoxItems { get; set; }
        private string _path;
        public ReactiveCommand<Unit, Unit> ConnectToServer { get; }
        public ReactiveCommand<Unit, Unit> DisconnectFromServer { get; }
        public ReactiveCommand<Unit, Unit> SendMessageToServer { get; }
        public ReactiveCommand<Unit, Unit> ShutOffServer { get; }
        private DirectoryParse _directoryParse;

        private TcpClient.TcpClient _client;

        public MainWindowViewModel()
        {
            _directoryParse = new DirectoryParse();
            listBoxItems = new ObservableCollection<string>();
            listBoxItems.Add("1");
            listBoxItems.Add("2");
            listBoxItems.Add("3");
            ConnectToServer = ReactiveCommand.Create(() =>
            {
                if (IpParse())
                {
                    _client = new TcpClient.TcpClient(ipAddress, port, _path);

                    if (_client.IsConnected) messageFromServer = "Подключение установлено успешно.";
                    else messageFromServer = "Подключение не удалось";
                }
                else messageFromServer = "Неправильно введён ip";
            });
            DisconnectFromServer = ReactiveCommand.Create(() =>
            {
                if (_client != null && _client.IsConnected) _client.CloseConnection();
                else messageFromServer = "Подключение с сервером ещё не установлено.";
            });
            SendMessageToServer = ReactiveCommand.Create(() =>
            {
                _path = listBoxItems[selectedIndexListBox];
                
                if (_client != null && _client.IsConnected && string.IsNullOrEmpty(_path))
                {
                    var buf = _directoryParse.StringParse(_client.SendMessageToServer(_path));
                    listBoxItems = buf;
                }
                else if (_client == null)
                    messageFromServer = "Подключение с сервером ещё не установлено.";
                else messageFromServer = "Не выбран путь.";
            });
            ShutOffServer = ReactiveCommand.Create(() =>
            {
                if (_client != null && _client.IsConnected) _client.CloseServer();
                else messageFromServer = "Подключение с сервером ещё не установлено.";
            });
        }

        private bool IpParse()
        {
            int count = 0;
            int chunkCount = 0;
            string buff = "";
            
            for (int i = 0; i < ipAddress.Length; i++)
            {
                if (ipAddress[i] == '.')
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
                    buff += ipAddress[i].ToString();

                    if (count > 3) return false;
                }
            }

            chunkCount += 1;
            
            if (Convert.ToInt32(buff) > 255 || chunkCount > 4) return false;
            
            return true;
        }
    }
}