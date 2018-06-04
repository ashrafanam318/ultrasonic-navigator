package DrawMap;

import javax.swing.JFrame;
import java.util.Scanner;
import java.io.*;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.media.opengl.GL2;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLProfile;
import javax.media.opengl.awt.GLCanvas;
import javax.swing.JFrame;

public class Map2D implements GLEventListener{
	
	static GLProfile profile = GLProfile.get(GLProfile.GL2);
    static GLCapabilities capabilities = new GLCapabilities(profile);
    // The canvas 
    static GLCanvas glcanvas = new GLCanvas(capabilities);
    
   public static void main(String[] args) {
	      //getting the capabilities object of GL2 profile
	   	  
	      
	      Map2D l = new Map2D();
	      //creating frame
	      glcanvas.addGLEventListener(l);
	      glcanvas.setSize(600, 400);
	      
	      final JFrame frame = new JFrame ("straight Line");
	      //adding canvas to frame
	      frame.getContentPane().add(glcanvas);
	      frame.setSize(frame.getContentPane().getPreferredSize());
	      frame.setVisible(true);
	      
	   }
        @Override
   public void display(GLAutoDrawable drawable) {
	  
                final GL2 gl = drawable.getGL().getGL2();
                
		   File f = new File("coordinates.txt");

            Scanner sc = null;   
            try {
                sc = new Scanner(f);
            } catch (FileNotFoundException ex) {
                System.err.println("File not found");
            }
            
            double x0 = 0.0;
            double y0 = 0.0;
            double x1 = 0.0;
            double y1 = 0.0;
            float colorBit1 = 0.0f;
            float colorBit2 = 0.0f;
            float colorBit3 = 0.0f;
            int modulator = 100;
            
            while(sc.hasNextLine()){
                
                String[] str = sc.nextLine().split(",");
                if(x1 > modulator || y1 > modulator) modulator *= 10;
                x1 = Double.parseDouble(str[0]) / modulator;
                y1 = Double.parseDouble(str[1]) / modulator;
                if(x1 >.99) x1 = .99;
                if(x1 < -.99) x1 = -.99;
                if(y1 >.99) y1 = .99;
                if(y1 < -.99) y1 = -.99;
                
//                if(x1 > x0) gl.glColor3f(0.0f, 1.0f, 0.0f); //green for right
//                else if(x1 < x0) gl.glColor3f(1.0f, 0.0f, 0.0f); //red for left
//                else gl.glColor3f(0.0f, 0.0f, 1.0f); // blue for forward
//                
                colorBit1 = (float) Math.random();
                colorBit2 = (float) Math.random();
                colorBit3 = (float) Math.random();
                
                gl.glColor3f(colorBit1, colorBit2, colorBit3);
                gl.glBegin (GL2.GL_LINES);
                gl.glVertex2d(x0, y0);
                gl.glVertex2d(x1, y1);
                gl.glEnd();
                x0 = x1; y0 = y1;
            }
   }
   
        @Override
   public void dispose(GLAutoDrawable arg0) {
      //method body
   }

   
   public void init(GLAutoDrawable drawable) {
      // method body
	   //4. drive the display() in a loop
	    }
   
   public void reshape(GLAutoDrawable arg0, int arg1, int arg2, int arg3, int arg4) {
      // method body
   }
   //end of main
}//end of classimport javax.media.opengl.GL2;
