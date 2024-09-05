#include "MyForm.h"
#include "rworker.h"

using namespace System;
using namespace System::Windows::Forms;

RWorker Worker;

[STAThread]
void main(array<String^>^ arg) {

    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    OSRGZ4sem::MyForm form; //WinFormsTest - имя вашего проекта
    Application::Run(% form);
}

System::Void OSRGZ4sem::MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
    Worker.startWorkX();
    Worker.startWorkY();

    if(Worker.isDataUpdated())  this->chart->Series[0]->Points->AddXY(Worker.getDataX(), Worker.getDataY());
    Sleep(500);


    /*this->chart->Series[0]->Points->Clear();
    this->chart->Series[0]->Points->AddXY(trackBar1->Value, trackBar1->Value);
    this->chart->Series[0]->Points->AddXY(trackBar2->Value, trackBar2->Value);
    this->chart->Series[0]->Points->AddXY(trackBar3->Value, trackBar3->Value);
    this->chart->Series[0]->Points->AddXY(trackBar1->Value, trackBar2->Value);
    this->chart->Series[0]->Points->AddXY(trackBar5->Value, trackBar6->Value);
    this->chart->Series[0]->Points->AddXY(trackBar6->Value, trackBar5->Value);*/
}

System::Void OSRGZ4sem::MyForm::trackBar1_Scroll(System::Object^ sender, System::EventArgs^ e)
{
    Worker.PARAMS.Ax = trackBar1->Value;
    label1->Text = String::Format("Ax = {0}", Worker.PARAMS.Ax);
}

System::Void OSRGZ4sem::MyForm::trackBar2_Scroll(System::Object^ sender, System::EventArgs^ e)
{
    Worker.PARAMS.Wx = trackBar2->Value;
    label2->Text = String::Format("Wx = {0}", Worker.PARAMS.Wx);
}

System::Void OSRGZ4sem::MyForm::trackBar3_Scroll(System::Object^ sender, System::EventArgs^ e)
{
    Worker.PARAMS.Dx = trackBar3->Value;
    label3->Text = String::Format("Dx = {0}", Worker.PARAMS.Dx);
}

System::Void OSRGZ4sem::MyForm::trackBar4_Scroll(System::Object^ sender, System::EventArgs^ e)
{
    Worker.PARAMS.Ay = trackBar4->Value;
    label4->Text = String::Format("Ay = {0}", Worker.PARAMS.Ay);
}

System::Void OSRGZ4sem::MyForm::trackBar5_Scroll(System::Object^ sender, System::EventArgs^ e)
{
    Worker.PARAMS.Wy = trackBar5->Value;
    label5->Text = String::Format("Wy = {0}", Worker.PARAMS.Wy);
}

System::Void OSRGZ4sem::MyForm::trackBar6_Scroll(System::Object^ sender, System::EventArgs^ e)
{
    Worker.PARAMS.Dy = trackBar6->Value;
    label6->Text = String::Format("Dy = {0}", Worker.PARAMS.Dy);
}

