/**
* $Header$
*/

import java.applet.Applet;
import java.awt.Graphics;
import java.awt.Color;

public class Appt extends Applet{

    String text = "text=Simple applet";

    public void init() {
        text = "init applet ...";
        setBackground(Color.gray);
    }
    public void start() {
        System.out.println("starting ...");
    }
    public void stop() {
        System.out.println("stopping ...");
    }
    public void destroy() {
        System.out.println("preparing to unload...");
    }
    public void paint(Graphics g){
        System.out.println("painting appplet ...");
        g.setColor(Color.blue);
        g.drawRect(0, 0,
                getSize().width -1,
                getSize().height -1);
        g.setColor(Color.red);
        g.drawString(text, 15, 25);
    }
}

