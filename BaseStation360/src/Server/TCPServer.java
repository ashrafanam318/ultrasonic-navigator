package Server;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author pritomn
 */
import java.io.*;
import java.net.*;

class BaseStationTCP {
  
    public static void main(String argv[]) throws Exception {

      ServerSocket welcomeSocket = new ServerSocket(6789);
      System.out.println("server deployed");

      File f = new File("coordinates.txt");
      PrintWriter pr = null;
      try {
        pr = new PrintWriter(f);
      } catch (FileNotFoundException ex) {
        System.err.println("File not found");
      }

      while (true) {

        Socket connectionSocket = welcomeSocket.accept();

        BufferedReader dis =
          new BufferedReader(new InputStreamReader(connectionSocket.getInputStream()));

        //     StringBuilder sb = new StringBuilder();
        double x = 0;
        double  y = 0;
        double r = 0;
        int signxL = 1;
        int signyL = 1;
        int signxR = -1;
        int signyR = 1;
        String lastChanged = "y";
        String prevDir = "NA";
        String dir = null;
        String[] data = null;

        while ((data = dis.readLine().split(",")) != null) {
          r = Double.parseDouble(data[0]);
          dir =data[1];

          if(dir.equals("f")){
              if(prevDir.equals("NA")){x += 0; y += r;}
              else if(prevDir.equals("r")){
                  x += r; y += 0;
              }
              else if(prevDir.equals("l")){
                  x += -r; y += 0;
              }
          } 

          else if(dir.equals("l")){
            if(prevDir.equals("NA")){
              signxL *= -1;
              x += signxL*r; y += 0;
              lastChanged = "x";
            }
            else if(prevDir.equals("l")){
              if(lastChanged.equals("y")){
                signxL *= -1;
                x += signxL*r; y += 0;
                lastChanged = "x";
              }
              else{ //x
                signyL *= -1;
                x += 0; y += signyL*r;
                lastChanged = "y";
              }
            }
            else if(prevDir.equals("r")){
              if(lastChanged.equals("y")){
                x += signxL*r; y += 0;
                lastChanged = "x";
              }
              else{ //lastChanged x
                x += 0; y += signyL*r;
                lastChanged = "y";
              }
            }
            prevDir = "l";
          }

          else if(dir.equals("r")){
            if(prevDir.equals("NA")){
              signxR *= -1;
              x += signxR*r; y += 0;
              lastChanged = "x";
            }
            else if(prevDir.equals("l")){

              if(lastChanged.equals("y")){
                x += signxR*r; y += 0;
                lastChanged = "x";
              }
              else{ //lastChanged x
                x += 0; y += signyR*r;
                lastChanged = "y";
              }
            }
            else if(prevDir.equals("r")){
              if(lastChanged.equals("y")){
                signxR *= -1;
                x += signxR*r; y += 0;
                lastChanged = "x";
              }
              else{ //x
                signyR *= -1;
                x += 0; y += signyR*r;
                lastChanged = "y";
              }
            }
            prevDir = "r";
          }     

        pr.write(x+","+y);
        System.out.println("From Client: "+x+","+y);
        pr.println();
        pr.flush();
  //        sb.append(data);
      } 
      pr.close();
    }

  }

}
