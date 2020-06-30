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

	public static void main(String[] args) throws IOException 
	{ 
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
	private String name; 
	final DataInputStream dis; 
	final DataOutputStream dos; 
	Socket s; 
	boolean isloggedin; 
	
	// constructor 
	public ClientHandler(Socket s, String name, 
							DataInputStream dis, DataOutputStream dos) { 
		this.dis = dis; 
		this.dos = dos; 
		this.name = name; 
		this.s = s; 
		this.isloggedin=true; 
	} 

	@Override
	public void run() { 

		String received; 
		while (true) 
		{ 
			try
			{ 
				// receive the string 
				received = dis.readUTF(); 
				
				System.out.println(received); 
				
				if(received.equals("logout")){ 
					this.isloggedin=false; 
					this.s.close(); 
					Server.ar.remove(this);
					break; 
				} 
				
				// break the string into message and recipient part 
				StringTokenizer st = new StringTokenizer(received, "#"); 
				String MsgToSend = st.nextToken(); 
				String recipient = st.nextToken(); 

				// search for the recipient in the connected devices list. 
				// ar is the vector storing client of active users 
				for (ClientHandler mc : Server.ar) 
				{ 
					// if the recipient is found, write on its 
					// output stream 
					if (mc.name.equals(recipient) && mc.isloggedin==true) 
					{ 
						mc.dos.writeUTF(this.name+" : "+MsgToSend); 
						break; 
					} 
				} 
			} catch (IOException e) { 
				
				e.printStackTrace(); 
			} 
			
		} 
		try
		{ 
			// closing resources 
			this.dis.close(); 
			this.dos.close(); 
			
		}catch(IOException e){ 
			e.printStackTrace(); 
		} 
	} 
} 



/*
class temp(int i){
//Iterator i = v.iterator();i.hasnext();char c = str.charAt(i);
	
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
}
					


*/


/*
char[] chars = str.toCharArray();
for (int i = 0, n = chars.length; i < n; i++) {
    char c = chars[i];
}
str.charAt(i);
*/
