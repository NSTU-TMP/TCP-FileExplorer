using System;
using System.Collections.Generic;
using System.ComponentModel.Design.Serialization;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Timers;
using DynamicData;

namespace Client.TcpClient;

public class TcpClient
{
    private string _path;
    private readonly System.Net.Sockets.TcpClient _clientForSendInformation;
    private readonly System.Net.Sockets.TcpClient _clientForData;
    private readonly NetworkStream _streamForSendInformation;
    private readonly NetworkStream _streamForData;
    private readonly Timer _timer;

    private const int Timeout = 2000;
    private const int Interval = 10000;

    public bool IsConnected = false;
    public bool IsFile = false;

    public TcpClient(string ip, int port)
    {
        var ipAddress = ip;
        
        try
        {
            _clientForSendInformation = new System.Net.Sockets.TcpClient(ipAddress, port);
            _streamForSendInformation = _clientForSendInformation.GetStream();
            
            var data = new byte[1024];
            
            data = System.Text.Encoding.UTF8.GetBytes("3");
            _streamForSendInformation.Write(data);

            data = new byte[3];
            
            int bytes = _streamForSendInformation.Read(data);
            string response = System.Text.Encoding.UTF8.GetString(data);
            
            _clientForData = new System.Net.Sockets.TcpClient(ipAddress, BitConverter.ToUInt16(data.Reverse().ToArray()));
            _streamForData = _clientForData.GetStream();

            _timer = new Timer();
            _timer.Interval = Interval;
            _timer.Elapsed += new ElapsedEventHandler(OnTimedEvent);
            _timer.Enabled = true;
            
            IsConnected = true;
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
        }
    }

    private void OnTimedEvent(object source, ElapsedEventArgs e)
    {
        // var data = System.Text.Encoding.UTF8.GetBytes("1");
        // _streamForSendInformation.Write(data);
        // data = new byte[1024];
        // var bytes = _streamForData.Read(data, 0, data.Length);
        // var response = System.Text.Encoding.UTF8.GetString(data, 0, bytes);
        //
        // var responseTimer = new Timer();
        // responseTimer.Interval = Timeout;
        // responseTimer.Elapsed += (sender, e) => 
        // {
        //     CloseConnection();
        //     responseTimer.Stop();
        //     responseTimer.Dispose();
        // };
        //
        // Console.WriteLine("All good!");
        // responseTimer.Start();
    }

    public string SendMessageToServer(string message)
    {
        try
        {
            var data = new byte[1024];

            data = System.Text.Encoding.UTF8.GetBytes("2" + message);
            
            _streamForSendInformation.Write(data, 0, data.Length);

            _streamForSendInformation.Read(data);

            if (data[0] == '2') return "Не получилось обработать данные...";

            data = new byte[1024];
            var dataList = new List<byte>();            
            var buf = new byte[2048];
            var bytes = 0;
            var count = 0;

            while (true)
            {
                bytes = _streamForData.Read(buf);

                if (bytes >= 2 && buf[bytes - 1] == '\n' && buf[bytes - 2] == '\r')
                {
                    dataList.AddRange(buf.Take(bytes - 2));
                    break;
                }
                
                dataList.AddRange(buf.Take(bytes));
            }

            if (dataList.Count >= 1)
            {
                if (dataList[0] == '1') IsFile = true;
                else IsFile = false;
            }

            var response = System.Text.Encoding.UTF8.GetString(dataList.ToArray());
            
            return response[1..response.Length];
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
        }

        return "Не получилось обработать данные...";
    }

    private void SendFlagToServer(MessageType requestType)
    {
        var data = System.Text.Encoding.UTF8.GetBytes("2");
        _streamForSendInformation.Write(data);
    }

    public void CloseConnection()
    {
        IsConnected = false;
        _clientForSendInformation.Close();
        _clientForData.Close();
    }

    public void CloseServer()
    {
        var data = System.Text.Encoding.UTF8.GetBytes("4");
        _streamForSendInformation.Write(data, 0, data.Length);
        
        _clientForData.Close();
        _clientForSendInformation.Close();
    }
}