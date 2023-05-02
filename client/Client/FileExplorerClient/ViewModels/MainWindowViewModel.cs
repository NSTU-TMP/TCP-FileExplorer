﻿using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data.SqlTypes;
using System.IO;
using System.Linq;
using System.Net;
using System.Reactive;
using System.Threading;
using System.Windows.Input;
using Avalonia.Collections;
using Avalonia.Controls;
using Avalonia.Interactivity;
using Client.TcpClient;
using DynamicData;
using ReactiveUI;
using ReactiveUI.Fody.Helpers;

namespace Client.ViewModels
{
    public class MainWindowViewModel : ViewModelBase
    {
        [Reactive] public string IpAddress { get; set; } = "127.0.0.1";
        [Reactive] private int Port { get; set; } = 4467;
        [Reactive] public string MessageFromServer { get; set; }
        [Reactive] public int SelectedIndexListBox { get; set; }
        [Reactive] public int SelectedIndexComboBox { get; set; }
        public ObservableCollection<string> ListBoxItems { get; } 
        public ObservableCollection<string> ComboBoxItems { get; set; }
        [Reactive] public string Path { get; set; }
        public ReactiveCommand<Unit, Unit> ConnectToServer { get; }
        public ReactiveCommand<Unit, Unit> JumpUp { get; }
        public ReactiveCommand<Unit, Unit> DoubleClick { get; }
        public ReactiveCommand<Unit, Unit> DisconnectFromServer { get; }
        public ReactiveCommand<Unit, Unit> SendMessageToServer { get; }
        public Parse directoryParser = new Parse();

        public ReactiveCommand<Unit, Unit> ShutOffServer { get; }

        private TcpClient.TcpClient _client;

        public MainWindowViewModel()
        {
            Console.WriteLine(Thread.CurrentThread.Name);
            
            ListBoxItems = new ObservableCollection<string>(new string[]{});
            ComboBoxItems = new ObservableCollection<string>();
            
            ConnectToServer = ReactiveCommand.Create(() =>
            {
                if (IpParse())
                {
                    _client = new TcpClient.TcpClient(IpAddress, Port);

                    if (_client.IsConnected)
                    {
                        MessageFromServer = "Подключение установлено успешно.";
                        Path = "/";
                        var s = directoryParser.StringParse(_client.SendMessageToServer(Path));
                        
                        ReplaceItems(s);
                    }
                    else MessageFromServer = "Подключение не удалось...";
                }
                else MessageFromServer = "Неправильно введён ip.";
            }, outputScheduler: RxApp.MainThreadScheduler);
            
            DisconnectFromServer = ReactiveCommand.Create(() =>
            {
                if (_client != null && _client.IsConnected)
                {
                    _client.CloseConnection();
                    ListBoxItems.Clear();
                    MessageFromServer = "Подключение разорвано.";
                }
                else MessageFromServer = "Подключение с сервером ещё не установлено.";
            });
            
            SendMessageToServer = ReactiveCommand.Create(() =>
            {
                Path = ListBoxItems[SelectedIndexListBox];

                if (_client != null && _client.IsConnected && !string.IsNullOrEmpty(Path))
                {
                    var str = _client.SendMessageToServer(Path);

                    if (_client.IsFile) MessageFromServer = str;
                    else
                    {
                        var buf = directoryParser.StringParse(str);
                        MessageFromServer = "Успешно";
                        ReplaceItems(buf);
                    }
                }
                else if (_client == null || _client.IsConnected == false)
                    MessageFromServer = "Подключение с сервером ещё не установлено.";
                else MessageFromServer = "Не выбран путь.";
            }, outputScheduler: RxApp.MainThreadScheduler);
            
            ShutOffServer = ReactiveCommand.Create(() =>
            {
                if (_client != null && _client.IsConnected) _client.CloseServer();
                else MessageFromServer = "Подключение с сервером ещё не установлено.";
            });

            DoubleClick = ReactiveCommand.Create(() =>
            {
                Path = ListBoxItems[SelectedIndexListBox];

                if (_client != null && _client.IsConnected && !string.IsNullOrEmpty(Path))
                {
                    var buf = directoryParser.StringParse(_client.SendMessageToServer(Path));
                    MessageFromServer = "Успешно.";
                    ReplaceItems(buf);
                }
                else if (_client == null || _client.IsConnected == false)
                    MessageFromServer = "Подключение с сервером ещё не установлено.";
                else MessageFromServer = "Не выбран путь.";
            });
            
            JumpUp = ReactiveCommand.Create(() =>
            {
                if (Path == "/") MessageFromServer = "Нельзя перейти назад. \nВы уже в корневой директории.";
                else
                {
                    Path = directoryParser.CutThePath(Path);

                    if (_client != null && _client.IsConnected && !string.IsNullOrEmpty(Path))
                    {
                        var buf = directoryParser.StringParse(_client.SendMessageToServer(Path));
                        MessageFromServer = "Успешно.";
                        ReplaceItems(buf);
                    }
                    else if (_client == null || _client.IsConnected == false)
                        MessageFromServer = "Подключение с сервером ещё не установлено.";
                    else MessageFromServer = "Не выбран путь.";
                }
            });
        }

        public void ListBoxDoubleClick()
        {
            Path = ListBoxItems[SelectedIndexListBox];

            if (_client != null && _client.IsConnected && !string.IsNullOrEmpty(Path))
            {
                var buf = directoryParser.StringParse(_client.SendMessageToServer(Path));
                    
                ReplaceItems(buf);
            }
            else if (_client == null || _client.IsConnected == false)
                MessageFromServer = "Подключение с сервером ещё не установлено.";
            else MessageFromServer = "Не выбран путь.";
        }
        
        
        private bool IpParse()
        {
            IPAddress ip;
            
            return IPAddress.TryParse(IpAddress, out ip);
        }

        private void ReplaceItems(IReadOnlyList<string> buff)
        {
            ListBoxItems.Clear();
            
            for (int i = 0; i < buff.Count; i++)
            {
                ListBoxItems.Add(buff[i]);
            }
        }
    }
}