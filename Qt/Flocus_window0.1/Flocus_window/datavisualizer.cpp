#include "datavisualizer.h"

DataVisualizer::DataVisualizer(QWidget *parent)
    : QWidget(parent)
    , width(500)
    , height(500)
{
    widget = new QVTKWidget(this);
    this->setMinimumSize(width,height);
    widget->resize(width,height);

    // TMP
    // Create a c-style image

    unsigned char cImage[width*height];
    for(unsigned int row = 0; row < height; ++row)
    {
        for(unsigned int col = 0; col < width; ++col)
        {
            cImage[row * width + col] = 255;
        }
    }

    imageImport = vtkSmartPointer<vtkImageImport>::New();
    imageImport->SetDataSpacing(1, 1, 1);
    imageImport->SetDataOrigin(0, 0, 0);
    imageImport->SetWholeExtent(0, width-1, 0, height-1, 0, 0);
    imageImport->SetDataExtentToWholeExtent();
    imageImport->SetDataScalarTypeToUnsignedChar();
    imageImport->SetNumberOfScalarComponents(1);
    imageImport->SetImportVoidPointer(cImage);
    imageImport->Update();

    // Create an actor
    vtkSmartPointer<vtkImageActor> actor =
    vtkSmartPointer<vtkImageActor>::New();
    actor->SetInput(imageImport->GetOutput());

    // Setup renderer
    vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->ResetCamera();

    // Setup render window
    vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    widget->SetRenderWindow(renderWindow);

}

void DataVisualizer::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
    this->resize(width,height);
    this->setMinimumSize(width,height);
    this->widget->resize(width,height);
}

void DataVisualizer::updateImage()
{
    this->setSize(width,height);

    imageImport->SetDataSpacing(1, 1, 1);
    imageImport->SetDataOrigin(0, 0, 0);
    imageImport->SetWholeExtent(0, width-1, 0, height-1, 0, 0);
    imageImport->SetDataExtentToWholeExtent();
    imageImport->SetDataScalarTypeToUnsignedChar();
    imageImport->SetNumberOfScalarComponents(1);
    imageImport->SetImportVoidPointer(cImage);
    imageImport->Update();
}

