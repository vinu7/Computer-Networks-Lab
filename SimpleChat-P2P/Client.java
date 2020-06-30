// Java implementation for multithreaded chat client
// Save file as Client.java

import java.io.*;
import java.net.*;
import java.util.*;//Scanner;

public class Client
{
	final static int ServerPort = 1234;

	public static /*synchronized*/ void main(String args[]) throws UnknownHostException, IOException
	{
		Scanner scn = new Scanner(System.in);

		// getting localhost ip
		InetAddress ip = InetAddress.getByName("localhost");

		// establish the connection
		Socket s = new Socket(ip, ServerPort);

		// obtaining input and out streams
		DataInputStream dis = new DataInputStream(s.getInputStream());
		DataOutputStream dos = new DataOutputStream(s.getOutputStream());

		// sendMessage thread
		Thread sendMessage = new Thread(new Runnable()
		{
			@Override
			public void run() {
				while (true) {

					// read the message to deliver.
					String msg = scn.nextLine();

					try {
						// write on the output stream
						dos.writeUTF(msg);
					} catch (IOException e) {
						System.exit(0);
						e.printStackTrace();
					}
				}
			}
		});

		// readMessage thread
		Thread readMessage = new Thread(new Runnable()
		{
			@Override
			public void run() {

				while (true) {
					try {
						// read the message sent to this client
						String msg = dis.readUTF();

						System.out.println(msg);

						StringTokenizer st = new StringTokenizer(msg, ":");
						String from = st.nextToken();
						String msgbody = st.nextToken();

						System.out.println(msgbody);
						if(msgbody.equals("request"))
						{
							String temp = scn.nextLine();
							
							try {
						// write on the output stream
								dos.writeUTF(temp);
							} catch (IOException e) {
								System.exit(0);
								e.printStackTrace();
								}
						}

					} catch (IOException e) {
						System.exit(0);
						e.printStackTrace();
					}
				}
			}
		});

		readMessage.start();
		sendMessage.start();


	}
}

//.equalsIgnoreCase("quit");t.join();;;ArrayList<Integer> arrli = new ArrayList<Integer>();
//List<Integer> Numbers = new ArrayList<>(); Numbers.add(0);.addAll;.isEmpty();.size(); .get(i); .set(i,"hello");
// .remove(i); .remove(object);

/*List<String> tvShows = new ArrayList<>();
Iterator<String> tvShowIterator = tvShows.iterator();
        while (tvShowIterator.hasNext()) {
            String tvShow = tvShowIterator.next();
            System.out.println(tvShow);
        }
*/