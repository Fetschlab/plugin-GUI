/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic outline for a simple desktop window.

  ==============================================================================
*/

#include "MainWindow.h"
#include <stdio.h>


//==============================================================================
MainWindow::MainWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colour(Colours::black),
                      DocumentWindow::allButtons)
{
    centreWithSize (500, 400);
    //setBounds(0,0,500,400);
    setResizable (true, false);
    
    
    
    // constraining size doesn't seem to work:
    //setResizeLimits(500, 400, 10000, 10000);
    //ComponentBoundsConstrainer* cbc = getConstrainer();
    //cbc->setMinimumWidth(300);
    //cbc->setMinimumHeight(200);

    // Create ProcessorGraph and AudioComponent, and connect them.
    // Callbacks will be set by the play button in the control panel
    processorGraph = new ProcessorGraph();
    audioComponent = new AudioComponent();
    audioComponent->connectToProcessorGraph(processorGraph);
    
    setContentComponent (new UIComponent(processorGraph, audioComponent), true, true);

    loadWindowBounds();
    setVisible (true);

}

MainWindow::~MainWindow()
{

  saveWindowBounds();
  processorGraph->saveState();

  audioComponent->disconnectProcessorGraph();

   deleteAndZero(processorGraph);
   deleteAndZero(audioComponent);

  setContentComponent (0);

}

void MainWindow::closeButtonPressed()
{ 
    if (audioComponent->callbacksAreActive()) {
      audioComponent->endCallbacks();
      processorGraph->disableProcessors();
    }

    JUCEApplication::getInstance()->systemRequestedQuit();

}

void MainWindow::saveWindowBounds()
{

    std::cout << "Saving window bounds." << std::endl;

    File file = File("./windowState.xml");

    XmlElement* xml = new XmlElement("MAINWINDOW");

    XmlElement* bounds = new XmlElement("BOUNDS");
    bounds->setAttribute("x",getScreenX());
    bounds->setAttribute("y",getScreenY());
    bounds->setAttribute("w",getWidth());
    bounds->setAttribute("h",getHeight());
    bounds->setAttribute("fullscreen",isFullScreen());

    xml->addChildElement(bounds);

    String error;
    
    if (! xml->writeToFile (file, String::empty))
        error = "Couldn't write to file";
    
    delete xml;
}

void MainWindow::loadWindowBounds()
{
  
    std::cout << "Loading window bounds." << std::endl;
    
    File file = File("./windowState.xml");

    XmlDocument doc (file);
    XmlElement* xml = doc.getDocumentElement();

    // if (xml == 0 || ! xml->hasTagName (T("MAINWINDOW")))
    // {
    //     delete xml;
    //    // return "Not a valid file.";
    // }

    String description;// = T(" ");

    forEachXmlChildElement (*xml, e)
    {

        int x = e->getIntAttribute("x");
        int y = e->getIntAttribute("y");
        int w = e->getIntAttribute("w");
        int h = e->getIntAttribute("h");

        bool fs = e->getBoolAttribute("fullscreen");

        setTopLeftPosition(x,y);
        getContentComponent()->setBounds(0,0,w,h);
        //setFullScreen(fs);

    }

    delete xml;
   // return "Everything went ok.";
}