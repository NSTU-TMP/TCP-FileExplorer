using System;
using System.Net;
using System.Net.Sockets;
using System.Timers;

namespace Client.TcpClient;

public class TcpClient
{
    private readonly string _ipAddress;
    private readonly int _port;
    private string _path;
    private System.Net.Sockets.TcpClient _client;
    private NetworkStream _stream;
    private Timer _timer;
    private MessageType _requestType;

    private const int Timeout = 2000;
    private const int Interval = 10000;

    public bool isConnected = false;
    
    public TcpClient(string ip, int port, string path)
    {
        _ipAddress = ip;
        _port = port;
        _path = path;

        try
        {
            _client = new System.Net.Sockets.TcpClient(_ipAddress, _port);
            _stream = _client.GetStream();

            _timer = new Timer();
            _timer.Interval = Interval;
            _timer.Elapsed += new ElapsedEventHandler(OnTimedEvent);
            _timer.Enabled = true;

            isConnected = true;
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
        }
    }

    private void OnTimedEvent(object source, ElapsedEventArgs e)
    {
        PingServer();
    }
    
    public string SendMessageToServer(string message)
    {
        try
        {
            byte[] data = System.Text.Encoding.UTF8.GetBytes(message);
            _stream.Write(data, 0, data.Length);

            data = new byte[1024];
            int bytes = _stream.Read(data, 0, data.Length);
            string response = System.Text.Encoding.UTF8.GetString(data, 0, bytes);
            
            return response;
        }
        catch (Exception e)
        {
            Console.WriteLine("Невъебическая ноунейм ошибка уровня SSS!!!");
        }

        return "Не получилось получить данные.";
    }

    private void PingServer()
    {
        SendMessageToServer(Convert.ToString(MessageType.PING));

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
    
    public void CloseConnection()
    {
        _client.Close();
    }
}