//	StandaloneApplet.java
//
//  Copyright (c) 1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	04/07/97 dgm	Original.
// ----------------------------------------------------------------------------

import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.net.*;
import java.io.*;
import java.util.*;

// --------------------------------------------------------------------------
//
public class StandaloneApplet
{
	public static void main(String[] args)
	{
		String	appletname = args[0];
		Class	appletclass;
		Applet	applet;

		try
		{
			appletclass = Class.forName(appletname);
		}
		catch (Throwable e)
		{
			System.err.println("Cannot find class " +
								appletname + " [1] ... Exit.");
			return;
		}
		if (appletclass == null) {
			System.err.println("Cannot find class " +
								appletname + " [2] ... Exit.");
			return;
		}

		try
		{
			applet = (Applet)appletclass.newInstance();
		}
		catch (Throwable e)
		{
			System.err.println("Cannot create " +
								appletclass.getName() + " [3] ... Exit.");
			return;
		}
		if (applet == null) {
			System.err.println("Cannot create " +
								appletclass.getName() + " [4] ... Exit.");
			return;
		}

		String[] applet_args = new String[args.length - 1];
		for (int i = 0 ; i < applet_args.length ; i++) {
			applet_args[i] = args[i + 1];
		}

		new StandaloneAppletFrame(applet, applet_args);
	}
}

// --------------------------------------------------------------------------
//
class StandaloneAppletFrame extends Frame implements Runnable,
													 AppletStub,
													 AppletContext
{
    private Applet		applet		= null;
    private String[]	appletArgs	= null;

    public StandaloneAppletFrame(Applet applet)
	{
		build(applet, null);
	}

    public StandaloneAppletFrame(Applet applet, String[] args)
	{
		build(applet, args);
	}

    protected void build(Applet app, String[] args)
	{
		addWindowListener(new WindowAdapter()
		{
			public void windowClosing(WindowEvent e)
			{
				setVisible(false);
				applet.stop();
				dispose();
				System.exit(0);
			}
		});
		applet = app;
		appletArgs = args;
		applet.setStub(this);
		setTitle(applet.getClass().getName());
		setLayout(new BorderLayout()); add("Center", applet); pack();
		(new Thread(this)).start();
	}
    
    public void run()
	{
		applet.init();
		pack(); validate();
		show();
		applet.start();
	}

    public boolean isActive()
	{
		return true;
	}
    
    public URL getDocumentBase()
	{
		String dir = System.getProperty( "user.dir" );
		String urlDir = dir.replace(File.separatorChar, '/');
		try
		{
			return new URL("file:" + urlDir + "/");
		}
		catch (MalformedURLException e)
		{
			return null;
		}
	}
    
    public URL getCodeBase()
	{
		// Hack: loop through each item in CLASSPATH, checking if
		// the appropriately named .class file exists there.  But
		// this doesn't account for .zip files.

		String path = System.getProperty("java.class.path");
		Enumeration tokenizer = new StringTokenizer(path, ":");
		String appletname = applet.getClass().getName();
		while (tokenizer.hasMoreElements()) {
			String dir = (String) tokenizer.nextElement();
			String filename = dir + File.separatorChar + appletname + ".class";
			File file = new File(filename);
			if (file.exists()) {
				String urlDir = dir.replace(File.separatorChar, '/');
				try
				{
					return new URL( "file:" + urlDir + "/" );
				}
				catch (MalformedURLException e)
				{
					return null;
				}
			}
		}
		return null;
	}

	public String getParameter(String name)
	{
		return null;
	}
    
    public void appletResize(int width, int height)
	{
		setSize(width, height);
	}

    public AppletContext getAppletContext()
	{
		return this;
	}

    public AudioClip getAudioClip(URL url)
	{
		//
		// This is an internal undocumented routine.  However, it
		// also provides needed functionality not otherwise available.
		// I suspect that in a future release, JavaSoft will add an
		// audio content handler which encapsulates this, and then
		// we can just do a getContent just like for images.
		//
		return new sun.applet.AppletAudioClip(url);
	}

    public Image getImage(URL url)
	{
		Toolkit tk = Toolkit.getDefaultToolkit();
		try
		{
			ImageProducer prod = (ImageProducer)url.getContent();
			return tk.createImage(prod);
	    }
		catch (IOException e)
		{
		    return null;
	    }
	}
    
    public Applet getApplet(String name)
	{
		if (name.equals(applet.getClass().getName())) {
		    return applet;
		}
		else {
			return null;
		}
	}
    
    public Enumeration getApplets()
	{
		Vector vector = new Vector();
		vector.addElement(applet);
		return vector.elements();
	}
    
    public void showDocument(URL url)
	{
	}
    
    public void showDocument(URL url, String target)
	{
	}
    
    public void showStatus(String status)
	{
	}
}
