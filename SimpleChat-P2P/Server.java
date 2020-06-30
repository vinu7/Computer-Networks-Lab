// Java implementation of Server side
// It contains two classes : Server and ClientHandler
// Save file as Server.java

import java.io.*;
import java.util.*;
import java.net.*;

// Server class
public class Server
{

	// Vector to store active clients
	static Vector<ClientHandler> ar = new Vector<>();

	// counter for clients
	static int i = 0;
	//static int serveron=1;

	public static void main(String[] args) throws IOException
	{
		Scanner sc = new Scanner(System.in);

		// server is listening on port 1234
		ServerSocket ss = new ServerSocket(1234);

		Socket s;

		// running infinite loop for getting
		// client request
		while (true)
		{
			// Accept the incoming request
			s = ss.accept();

			System.out.println("New client request received : " + s);

			// obtain input and output streams
			DataInputStream dis = new DataInputStream(s.getInputStream());
			DataOutputStream dos = new DataOutputStream(s.getOutputStream());

			System.out.println("Creating a new handler for this client...");

			// Create a new handler object for handling this request.
			ClientHandler mtch = new ClientHandler(s,"client " + i, dis, dos);

			// Create a new Thread with this object.
			Thread t = new Thread(mtch);

			System.out.println("Adding this client to active client list");

			// add this client to active clients list
			ar.add(mtch);

			// start the thread.
			t.start();

			// increment i for new client.
			// i is used for naming only, and can be replaced
			// by any naming scheme
			i++;

		}
	}
}

// ClientHandler class
class ClientHandler implements Runnable
{
	Scanner scn = new Scanner(System.in);
	 String name;
	final DataInputStream dis;
	final DataOutputStream dos;
	Socket s;
	boolean isloggedin;
	boolean welcome;
	int flag=0;
	char ch;
	Vector<String> friends = new Vector<String>();

	// constructor
	public ClientHandler(Socket s, String name, DataInputStream dis, DataOutputStream dos) {
		this.dis = dis;
		this.dos = dos;
		this.name = name;
		this.s = s;
		this.isloggedin=true;
		this.welcome=true;

	}

	@Override
	public void run() {

		String received;
		while (true)
		{
			try
			{

				//for client joining a room,
				if(this.welcome){
					//client will get list of all other active clients
					for (ClientHandler mc : Server.ar){
						if(mc.name != this.name){this.dos.writeUTF(mc.name+":"+"is active");}
					}
				}
				this.welcome=false;

				// receive the string
				received = dis.readUTF();

				System.out.println(received);

				if(received.equals("logout")){
					this.isloggedin=false;
					flag=0;
					for (ClientHandler mc : Server.ar)
					{
						if(flag==1)
						{
							ch=mc.name.charAt(7);//=mc.name.charAt(7)-1;
							char[] myNameChars = mc.name.toCharArray();
							ch-=1;
							myNameChars[7] = ch;
							mc.name = String.valueOf(myNameChars);
						}
						if(mc.name == this.name && flag==0)
						{
							flag=1;
							//this.dos.writeUTF(mc.name+":"+"is active");
						}
					}
					Server.i--;
					this.s.close();
					Server.ar.remove(this);
					break;
				}

				// break the string into message and recipient part
				StringTokenizer st = new StringTokenizer(received, "#");
				//int count=st.countTokens();hasMoreTokens();
				/*for (int i = 0; i <count; i++) 
            System.out.println("token at [" + i + "] : "
                               + geeks3.nextToken()); 
  				*/

				String MsgToSend = st.nextToken();
				String recipient = st.nextToken();

				//---------------
				if(MsgToSend.equals("request"))
				{
					for (ClientHandler mc : Server.ar)
					{
					// if the recipient is found, write on its
					// output stream
						if (mc.name.equals(recipient) && !recipient.equals(this.name))
						{
							mc.dos.writeUTF(this.name+":"+MsgToSend);
							break;
						}
					}
					String res=dis.readUTF();
					if(res.equals("accept"))
					{

						for (ClientHandler mc : Server.ar)
						{
					
							if (mc.name.equals(recipient) )//&& !recipient.equals(this.name))
							{
								//mc.dos.writeUTF(this.name+" : "+MsgToSend);
								mc.friends.add(this.name);
								break;
							}
						}


						//mc.friends.add(this.name);
						this.friends.add(recipient);
					}
				}
				else
				{
				//------------
				// search for the recipient in the connected devices list.
				// ar is the vector storing client of active users
				for (ClientHandler mc : Server.ar)
				{
					// if the recipient is found, write on its
					// output stream
					if (mc.name.equals(recipient) && !recipient.equals(this.name) )//&& mc.friends.contains(this.name))
					{
						mc.dos.writeUTF(this.name+":"+MsgToSend);
						break;
					}
				}

				}
			} catch (IOException e) {
				System.exit(0);
				e.printStackTrace();
			}

		}
		try
		{
			// closing resources
			this.dis.close();
			this.dos.close();

		}catch(IOException e){
			System.exit(0);
			e.printStackTrace();
		}
	}
}

//vector
//Vector<Vector<String>> s = new Vector<Vector<String>>();
//Vector vec = new Vector();
//addElement(Object element);int size();Object elementAt(int index);Object firstElement();Object get(int index):
//Collections.sort(vector);Object obj = vector.remove(2);;Vector.insertElementAt(element,index); .contains(ele);
//Iterator value = vector.iterator(); 
//while (value.hasNext()) { 
  //          System.out.println(value.next()); 
    //    } 


//Thread
//getName;getPriority;isAlive;join:waits;run;sleep;start;getId;
//currentThread() ;