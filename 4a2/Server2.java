// Java implementation of Server side 
// It contains two classes : Server and ClientHandler 
// Save file as Server.java 

import java.io.*; 
import java.text.*; 
import java.util.*; 
import java.net.*; 

// Server class 
public class Server2
{ 
	public static void main(String[] args) throws IOException 
	{ 
		// server is listening on port 5056 
		ServerSocket ss = new ServerSocket(5056); 
		
		// running infinite loop for getting 
		// client request 
		ArrayList<Socket> list=new ArrayList<Socket>();



		while (true) 
		{ 
			Socket s = null; 
			
			try
			{ 
				// socket object to receive incoming client requests 
				s = ss.accept(); 
				
				list.add(s);
				System.out.println(list);

				System.out.println("A new client is connected : " + s); 
				
				// obtaining input and out streams 
				DataInputStream dis = new DataInputStream(s.getInputStream()); 
				DataOutputStream dos = new DataOutputStream(s.getOutputStream()); 
				
				System.out.println("Assigning new thread for this client"); 

				// create a new thread object 
				Thread t = new ClientHandler(s, dis, dos,list); 

				// Invoking the start() method 
				t.start(); 
				
			} 
			catch (Exception e){ 
				s.close(); 
				e.printStackTrace(); 
			} 
		} 
	} 
} 

// ClientHandler class 
class ClientHandler extends Thread 
{ 
	DateFormat fordate = new SimpleDateFormat("yyyy/MM/dd"); 
	DateFormat fortime = new SimpleDateFormat("hh:mm:ss"); 
	//final
	 DataInputStream dis; 
	//final
	 DataOutputStream dos; 
	//final 
	Socket s,stemp; 
	
	public ArrayList<Socket> list=new ArrayList<Socket>();


	// Constructor 
	public ClientHandler(Socket s, DataInputStream dis, DataOutputStream dos,ArrayList<Socket> list) 
	{ 
		this.s = s; 
		this.dis = dis; 
		this.dos = dos; 
		this.list = list;

		System.out.println(list);


	} 

	@Override
	public void run() 
	{ 
		String received; 
		String toreturn; 
		while (true) 
		{ 
			try { 

				// Ask user what he wants 
				dos.writeUTF("What do you want?[Date | Time]..\n"+ 
							"Type Exit to terminate connection."); 
				
				// receive the answer from client 
				received = dis.readUTF(); 
				
				if(received.equals("Exit")) 
				{ 
					/*System.out.println("Client " + this.s + " sends exit..."); 
					System.out.println("Closing this connection."); 
					this.s.close(); 
					System.out.println("Connection closed"); 
					break; 
					*/
					System.out.println("Hello"); 
					int n=list.size();
					aa:
					for(int i=0;i<n;i++)  
           			{  
            			//System.out.println(list.get(i));
            			stemp = list.get(i);
            			if(stemp.equals(this.s))
            			{
            				System.out.println("Hellloouuyui"); 
            				//this.s.close();
           					i++;
            				for(int j=i;j<n;j++) 
            				{

            					System.out.println("Hellloouuyui"); 
            				
            					stemp = list.get(j);

            					stemp.close();
            					list.remove(j);
            				}
            				break aa;
            			}

           			}  
           			this.s.close();
           			System.out.println("Connection closed"); 
					break; 
					
				} 
				
				// creating Date object 
				Date date = new Date(); 
				
				// write on output stream based on the 
				// answer from the client 
				switch (received) { 
				
					case "Date" : 
						toreturn = fordate.format(date); 
						dos.writeUTF(toreturn); 
						break; 
						
					case "Time" : 
						toreturn = fortime.format(date); 
						dos.writeUTF(toreturn); 
						break; 
						
					default: 
						dos.writeUTF("Invalid input"); 
						break; 
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
