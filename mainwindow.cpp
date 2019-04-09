#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkPolyData.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkGenericOpenGLRenderWindow.h>

#include <vtkRegularPolygonSource.h>
#include <vtkPyramid.h>
#include <QVTKOpenGLWidget.h>
#include <vtkUnstructuredGrid.h>
#include <vtkMapper.h>
#include <QComboBox>
#include <string.h>
#include <QColorDialog>
#include <vtkLightActor.h>
#include <vtkLight.h>
#include <vtkLightCollection.h>
#include <QFileDialog>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkProp.h>
#include <vtkSphereSource.h>
#include <vtkShrinkFilter.h>
#include <vtkPlane.h>
#include <vtkClipDataset.h>
#include <QSlider>
#include <QCheckBox>
#include <vtkAlgorithm.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // standard call to setup Qt UI (same as previously)
    ui->setupUi( this );
	
	// Now need to create a VTK render window and link it to the QtVTK widget
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
        ui->qvtkWidget->SetRenderWindow( renderWindow ); // note that vtkWidget is the name I gave to my QtVTKOpenGLWidget in Qt creator
	
	// Now use the VTK libraries to render something. To start with you can copy-paste the cube example code, there are comments to show where the code must be modified.

                // Create a cube using a vtkCubeSource
                vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();

                // Create a MAPPER that will hold the cube's geometry in a format suitable for rendering
                modelData = (vtkSmartPointer<vtkAlgorithm>) cubeSource; // ??????
                vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
                mapper->SetInputConnection( cubeSource->GetOutputPort() );


                // Create a RENDERER and render a window
                this->mapper = mapper;

                renderer = vtkSmartPointer<vtkRenderer>::New();
                ui->qvtkWidget->GetRenderWindow()->AddRenderer( this->renderer );

                vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New(); // set the color ???
                renderer->SetBackground( colors->GetColor3d( "Silver" ).GetData() );// set the background color


                /*
                // Create an actor that is used to set the cube's properties for rendering and place it in the window
                vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
                actor->SetMapper(mapper);
                actor->GetProperty()->EdgeVisibilityOn();

                vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
                actor->GetProperty()->SetColor( colors->GetColor3d("Red").GetData() );

                // Add the actor to the scene
                renderer->AddActor(actor);
                renderer->SetBackground( colors->GetColor3d("Silver").GetData() );
                */

                update();

                // Setup the renderers's camera
                renderer->ResetCamera();
                renderer->GetActiveCamera()->Azimuth(30);
                renderer->GetActiveCamera()->Elevation(30);
                renderer->ResetCameraClippingRange();

                // create and add a light to the renderer
                vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
                light->SetLightTypeToSceneLight();
                light->SetPosition( 5, 5, 15 );
                light->SetPositional( true ); // required for vtkLightActor below
                light->SetConeAngle( 10 );
                light->SetFocalPoint( 0, 0, 0 );
                light->SetDiffuseColor( 1, 1, 1 );
                light->SetAmbientColor( 1, 1, 1 );
                light->SetSpecularColor( 1, 1, 1 );
                light->SetIntensity( 0.5 );

                ui->intensitySlider->setSliderPosition( 100 ); // check slider !!!!!!!!!!! !!!! !!!!!!!!!
                ui->shrinkSlider->setSliderPosition(100);

                // Render
                ui->qvtkWidget->GetRenderWindow()->Render();

                // add the light to the renderer
                renderer->AddLight( light );
                connect( ui->changeColourButton, &QPushButton::released, this, &MainWindow::changeColour );
                connect( ui->changeBackgroundButton, &QPushButton::released, this, &MainWindow::changeBackground );
                connect( ui->intensitySlider, &QAbstractSlider::sliderMoved, this, &MainWindow::intensityChanged );
                connect(ui->shrinkSlider, &QAbstractSlider::sliderMoved, this, &MainWindow::update);



                connect( ui->clipFilterButton, &QPushButton::released, this, &MainWindow::clipFilterChanged );   //



                //connect(ui->edgesButton, &QPushButton::released, this, &MainWindow::changeEdges);
               // connect( ui->shrinkFilterButton, &QPushButton::released, this, &MainWindow::shrinkFilterChanged ); //
              //  connect( ui->resetButton, &QPushButton::released, this, &MainWindow::resetView );
				

                update();

                renderer->SetBackground(colors->GetColor3d("Silver").GetData());
                actor->GetProperty()->SetColor(1.0, 0.0, 0.0);




/*
        //--------------------------------------------- Code From Example 1 --------------------------------------------------------------------------
	// Create a cube using a vtkCubeSource
	vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();

	// Create a mapper that will hold the cube's geometry in a format suitable for rendering
	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputConnection( cubeSource->GetOutputPort() );

	// Create an actor that is used to set the cube's properties for rendering and place it in the window
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOn();

	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
	actor->GetProperty()->SetColor( colors->GetColor3d("Red").GetData() );

	// Create a renderer, and render window
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	//vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();		// ###### We've already created the renderWindow this time ######
	ui->qvtkWidget->GetRenderWindow()->AddRenderer( renderer );									// ###### ask the QtVTKOpenGLWidget for its renderWindow ######

	// Link a renderWindowInteractor to the renderer (this allows you to capture mouse movements etc)  ###### Not needed with Qt ######
	//vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//renderWindowInteractor->SetRenderWindow( ui->vtkWidget->GetRenderWindow() );				

	// Add the actor to the scene
	renderer->AddActor(actor);
	renderer->SetBackground( colors->GetColor3d("Silver").GetData() );

	// Setup the renderers's camera
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->ResetCameraClippingRange();

	// Render and interact
	//renderWindow->Render();					// ###### Not needed with Qt ######
	//renderWindowInteractor->Start();			// ###### Not needed with Qt ######
        //--------------------------------------------- /Code From Example 1 -------------------------------------------------------------------------
*/

}

















//--------------------------------------------- /UPDATE FUNCTION -------------------------------------------------------------------------

void MainWindow::update()
{
    vtkSmartPointer<vtkAlgorithm> datapoint = modelData;


if (clipTrue ==1)
{
    // this will apply a clipping plane whose normal is the x-axis that crosses the x-axis at x=0
    vtkSmartPointer<vtkPlane>  planeLeft = vtkSmartPointer<vtkPlane>::New();
    planeLeft->SetOrigin( 0.0, 0.0, 0.0 );
    planeLeft->SetNormal( -1.0, 0.0, 0.0 );

    // apply the clipping filter
    vtkSmartPointer<vtkClipDataSet> clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
    clipFilter->SetInputConnection( datapoint->GetOutputPort() ) ;
    clipFilter->SetClipFunction( planeLeft.Get() );
    //MAPPER->SetInputConnection( clipFilter->GetOutputPort() ); //??????

    datapoint = (vtkSmartPointer<vtkAlgorithm>) clipFilter;
}


if (edgesTrue == 1)
{
        //vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->EdgeVisibilityOn();
}

if (edgesTrue == 0)
{
        //vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->EdgeVisibilityOff();
}

if (shrinkTrue ==1)
{

    double shrinkvalue = ui->shrinkSlider->value(); // check this slider !!!!!!!!!!!!!!!!!!!!!!!

    vtkSmartPointer<vtkShrinkFilter> shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
    shrinkFilter->SetInputConnection(datapoint->GetOutputPort());
    shrinkFilter->SetShrinkFactor(shrinkvalue / 100);
    shrinkFilter->Update();

    datapoint = (vtkSmartPointer<vtkAlgorithm>) shrinkFilter;
    ui->qvtkWidget->GetRenderWindow()->Render();
}

    // this is the output from the data source
    // all items in pipeline are subclasses of vtkAlgorithm


    // Create a mapper that will hold the geometry in a format suitable for rendering
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputConnection( datapoint->GetOutputPort() );

    // Create an actor that is used to set properties for rendering and place it in the window
    actor->SetMapper( mapper );
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    actor->GetProperty()->SetColor(modelColour.redF(), modelColour.greenF(), modelColour.blueF());


    // Create a renderer, and render window
    renderer = vtkSmartPointer<vtkRenderer>::New();
    //vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();		// ###### We've already created the renderWindow this time ######								// ###### ask the QtVTKOpenGLWidget for its renderWindow ######
    // remove previous actors
    renderer->RemoveAllViewProps();

    // Add the actor to the scene
    renderer->AddActor(actor);
    renderer->SetBackground( backColour.redF(), backColour.greenF(), backColour.blueF()); // colors??????????????????????????????????

    // Setup the renderers's camera
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
    ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
}

//--------------------------------------------- /UPDATE FUNCTION -------------------------------------------------------------------------
















MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::changeColour() //try and make it start at the correct intensity once you have changed the colour
{
    // get colour selection from user
    QColor colour = QColorDialog::getColor( modelColour, this );

    if( colour.isValid() )
    {
        modelColour = colour;
        update();
    }

    ui->qvtkWidget->GetRenderWindow()->Render();
    double xvalue=ui->intensitySlider->value();
    intensityChanged(xvalue);
}

void MainWindow::intensityChanged( int value ) {
        // get list of model lights
        vtkLightCollection* lights = renderer->GetLights();

        // loop through them and adjust the intensity
        lights->InitTraversal();
        vtkLight *light;

        while( (light = lights->GetNextItem()) )
        {
              light->SetIntensity( (double) value/99 );
        }

        ui->qvtkWidget->GetRenderWindow()->Render();
}



void MainWindow::clipFilterChanged(int value)
{

    if (ui->clipFilterButton->isChecked())
    {
        clipTrue = 1;
        update();
    }
    else
    {
        clipTrue = 0;
        update();
    }
    ui->qvtkWidget->GetRenderWindow()->Render();
}


void MainWindow::on_actionOpen_triggered()
{
    // Run a file open dialog and get the filename to open
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open STL File"), "./", tr("STL Files (*.stl)"));

   if( fileName.length() <= 0 ) return;

    // Load STL file
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName( fileName.toLatin1().data() );
    reader->Update();

    // save the source algorithm so that filters can be applied later
    modelData = (vtkSmartPointer<vtkAlgorithm>) reader;

    update();

    // todo: need to adjust light position according to model dimensions

    // todo: need to adjust default camera position according to model dimensions

    ui->qvtkWidget->GetRenderWindow()->Render();
}





void MainWindow::shrinkFilterChanged()
{
    if (ui->shrinkFilterButton->isChecked())
    {
        shrinkTrue = 1;
        update();
    }
    else
    {
        shrinkTrue = 0;
        update();
    }
    ui->qvtkWidget->GetRenderWindow()->Render();
}





void MainWindow::resetView()
{
    this->renderer->ResetCamera();
            //this->renderer->GetActiveCamera()->SetPosition( 5, 5, 15 );
            //this->renderer->GetActiveCamera()->SetFocalPoint( 0, 0, 0 );
            //this->renderer->GetActiveCamera()->SetViewUp( 0, 1, 0 );
            this->renderer->ResetCameraClippingRange();
                        this->renderer->GetActiveCamera()->Azimuth(30);
                        this->renderer->GetActiveCamera()->Elevation(30);
            ui->clipFilterButton->setChecked(false);
            ui->shrinkFilterButton->setChecked(false);
            ui->edgesButton->setChecked(false);
           /* vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
                        backColour = colors->G
            renderer->SetBackground( colors->GetColor3d( "Silver" ).GetData() );
            actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
                        */
                        backColour = QColor(192, 192, 192);
                        modelColour = QColor(255, 0, 0);
                        update();
            this->ui->qvtkWidget->GetRenderWindow()->Render();

}







void MainWindow::changeBackground()
{
    QColor bcolour = QColorDialog::getColor( backColour, this );  // get colour selection from user

    if( bcolour.isValid() )
    {
        backColour = bcolour;
        update();
    }

    ui->qvtkWidget->GetRenderWindow()->Render();
}





void MainWindow::changeEdges(int arg1)
{
    if (ui->edgesButton->isChecked())
        {
                edgesTrue = 1;
                update();
        }
    else
        {
                edgesTrue = 0;
                update();
        }
    ui->qvtkWidget->GetRenderWindow()->Render();
}



