using System;
using System.Net;
using System.Net.Sockets;
using System.Timers;

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

    public readonly bool IsConnected = false;

    public TcpClient(string ip, int port)
    {
        var ipAddress = ip;
        
        try
        {
            _clientForSendInformation = new System.Net.Sockets.TcpClient(ipAddress, port);
            _streamForSendInformation = _clientForSendInformation.GetStream();
            
            var data = new byte[1024];
            int bytes = _streamForSendInformation.Read(data, 0, data.Length);
            string response = System.Text.Encoding.UTF8.GetString(data, 0, bytes);

            _clientForData = new System.Net.Sockets.TcpClient(ipAddress, Convert.ToInt16(response));
            _streamForData = _clientForData.GetStream();
            
            
            _timer = new Timer();
            _timer.Interval = Interval;
            _timer.Elapsed += new ElapsedEventHandler(OnTimedEvent);
            _timer.Enabled = true;

            IsConnected = true;
        }
        catch (Exception e)
        {
            Console.WriteLine("Ошибка ээээээ шо делать????");
        }
    }

    private void OnTimedEvent(object source, ElapsedEventArgs e)
    {
        var data = System.Text.Encoding.UTF8.GetBytes(((int)MessageType.PING).ToString());
        _streamForSendInformation.Write(data);
        data = new byte[1024];
        var bytes = _streamForData.Read(data, 0, data.Length);
        var response = System.Text.Encoding.UTF8.GetString(data, 0, bytes);

        var responseTimer = new Timer();
        responseTimer.Interval = Timeout;
        responseTimer.Elapsed += (sender, e) => 
        {
            CloseConnection();
            responseTimer.Stop();
            responseTimer.Dispose();
        };
        
        responseTimer.Start();
    }

    public string SendMessageToServer(string message)
    {
        try
        {
            var data = System.Text.Encoding.UTF8.GetBytes(message);
            _streamForSendInformation.Write(data, 0, data.Length);

            data = new byte[1024];
            var bytes = _streamForData.Read(data, 0, data.Length);
            var response = System.Text.Encoding.UTF8.GetString(data, 0, bytes);
            
            return response;
        }
        catch (Exception e)
        {
            Console.WriteLine("Невъебическая ноунейм ошибка уровня SSS!!!");
        }

        return "Не получилось получить данные...";
    }

    public void CloseConnection()
    {
        _clientForSendInformation.Close();
        _clientForData.Close();
    }

    public void CloseServer()
    {
        var data = System.Text.Encoding.UTF8.GetBytes(((int)MessageType.SHUT_OFF_SERVER).ToString());
        _streamForSendInformation.Write(data, 0, data.Length);
        
        _clientForData.Close();
        _clientForSendInformation.Close();
    }
}