import java.io.*;
import java.net.*;

public class Client
{

	private Socket sock=null;
	private DataInputStream input =null;
	private DataOutputStream output=null;

	public Client(String address,int port)
	{
		try
		{
			sock=new Socket(address,port);
			System.out.println("connected");
			input = new DataInputStream(System.in);
			output=new DataOutputStream(sock.getOutputStream());

		}
		catch(UnknownHostException u)
		{
			System.out.println(u);
		}
		catch(IOException i)
		{
			System.out.println(i);	
		}

		String line="";
		while(!line.equals("Over"))
		{
			try
			{
				line=input.readLine();
				output.writeUTF(line);
			}
			catch(IOException i)
			{
				System.out.println(i);
			}

		}

		try
        { 
            input.close(); 
            output.close(); 
            sock.close(); 
        } 
        catch(IOException i) 
        { 
            System.out.println(i); 
        } 
	}



	public static void main(String args[])
	{
		Client client=new Client("127.0.0.1",9999);
	}
}


