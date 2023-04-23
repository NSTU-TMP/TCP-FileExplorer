﻿using System;
using System.Reactive;
using ReactiveUI;
using ReactiveUI.Fody.Helpers;

namespace Client.ViewModels
{
    public class MainWindowViewModel : ViewModelBase
    {
        [Reactive] public string ipAddress { get; set; } = "127.0.0.1";
        [Reactive] public int port { get; set; } = 1024;
        [Reactive] public string messageFromServer { get; set; }
        private string _path;
        public ReactiveCommand<Unit, Unit> ConnectToServer { get; }
        public ReactiveCommand<Unit, Unit> DisconnectFromServer { get; }
        public ReactiveCommand<Unit, Unit> SendMessageToServer { get; }

        private TcpClient.TcpClient _client;

        public MainWindowViewModel()
        {
            ConnectToServer = ReactiveCommand.Create(() =>
            {
                _client = new TcpClient.TcpClient(ipAddress, port, _path);

                if (_client.isConnected) messageFromServer = "Подключение установлено успешно.";
                else messageFromServer = "Подключение не удалось";
            });
            DisconnectFromServer = ReactiveCommand.Create(() =>
            {
                if (_client != null && _client.isConnected) _client.CloseConnection();
                else messageFromServer = "Подключение с сервером ещё не установлено.";
            });
            SendMessageToServer = ReactiveCommand.Create(() =>
            {
                if (_client != null && _client.isConnected && string.IsNullOrEmpty(_path)) messageFromServer = _client.SendMessageToServer(_path);
                else messageFromServer = "Подключение с сервером ещё не установлено.";
            });
        }
    }
}