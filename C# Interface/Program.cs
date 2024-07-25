using System;
using System.IO.Pipes;
using System.Text;

class Interface
{
    static void Main()
    {
        using (var client = new NamedPipeClientStream(".", "bozo", PipeDirection.Out))
        {
            client.Connect();

            while (true)
            {
                Console.Write("Input << ");
                string input = Console.ReadLine();
                byte[] buffer = Encoding.UTF8.GetBytes(input);
                client.Write(buffer, 0, buffer.Length);
            }
        }
    }
}
