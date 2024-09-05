//---------------------------------------------------------------------------
#pragma hdrstop
#include "rworker.h"

RWorker::RWorker()
{
    //Событие с ручным управлением, изначально активное
    hUpdateEvtX = CreateEvent(NULL, TRUE, TRUE, NULL);
    hUpdateEvtY = CreateEvent(NULL, TRUE, TRUE, NULL);
    //Событие с ручным управлением, изначально неактивное
    hStopEvtX = CreateEvent(NULL, TRUE, FALSE, NULL);
    hStopEvtY = CreateEvent(NULL, TRUE, FALSE, NULL);
    //Мютекс, пока свободный
    hMutex = CreateMutex(NULL, FALSE, "");

    DATA.x = 0;
    DATA.y = 0;
    PARAMS.Ax = 0;
    PARAMS.Wx = 0;
    PARAMS.Dx = 0;
    PARAMS.Ay = 0;
    PARAMS.Wy = 0;
    PARAMS.Dy = 0;

    hThread1 = INVALID_HANDLE_VALUE; //поток пока не запущен
    hThread2 = INVALID_HANDLE_VALUE;
}

RWorker::~RWorker()
{
    stopWorkX();  //если работали, останавливаемся
    stopWorkY();
    CloseHandle(hUpdateEvtX);
    CloseHandle(hUpdateEvtY);
    CloseHandle(hStopEvtX);
    CloseHandle(hStopEvtY);
    CloseHandle(hMutex);
}



void RWorker::setParams(RWorker::parameters val)
{

    PARAMS.Ax = val.Ax;
    PARAMS.Wx = val.Wx;
    PARAMS.Dx = val.Dx;
    PARAMS.Ay = val.Ay;
    PARAMS.Wy = val.Wy;
    PARAMS.Dy = val.Dy;
    PARAMS.t = val.t;
    input.Params = PARAMS;

}

void RWorker::increaseTime()
{
    input.Params.t++;
}




void    RWorker::startWorkX()
{
    if (hThread1 != INVALID_HANDLE_VALUE) return; //уже запущен
    ResetEvent(hStopEvtX); //перед началом работы сбрасываем флаг остановки
    hThread1 = CreateThread(NULL, 0, ThreadFuncX, this, 0, NULL); //запускаемся в работу
}

void    RWorker::startWorkY()
{
    if (hThread2 != INVALID_HANDLE_VALUE) return; //уже запущен
    ResetEvent(hStopEvtY); //перед началом работы сбрасываем флаг остановки
    hThread2 = CreateThread(NULL, 0, ThreadFuncY, this, 0, NULL); //запускаемся в работу
}

void    RWorker::stopWorkX()
{
    if (hThread1 == INVALID_HANDLE_VALUE) return; //не запущен
    SetEvent(hStopEvtX); //взводим флаг остановки
    //ждем, когда поток заметит наш флаг и остановится
    if (WaitForSingleObject(hThread1, 5000) != WAIT_OBJECT_0)
    {
        //прождали пять минут, не остановился, "приболел" видимо, пристрелим, чтобы не мучался
        TerminateThread(hThread1, 1);
    }
    CloseHandle(hThread1);
    hThread1 = INVALID_HANDLE_VALUE;
}

void    RWorker::stopWorkY()
{
    if (hThread2 == INVALID_HANDLE_VALUE) return; //не запущен
    SetEvent(hStopEvtY); //взводим флаг остановки
    //ждем, когда поток заметит наш флаг и остановится
    if (WaitForSingleObject(hThread2, 5000) != WAIT_OBJECT_0)
    {
        //прождали пять минут, не остановился, "приболел" видимо, пристрелим, чтобы не мучался
        TerminateThread(hThread2, 1);
    }
    CloseHandle(hThread2);
    hThread2 = INVALID_HANDLE_VALUE;
}

int RWorker::getDataX()
{
    RMutexLocker guard(hMutex);
    ResetEvent(hUpdateEvtX);
    return DATA.x;
}

int RWorker::getDataY()
{
    RMutexLocker guard(hMutex);
    ResetEvent(hUpdateEvtY);
    return DATA.y;
}

void RWorker::setDataX(double val)
{
    RMutexLocker guard(hMutex);
    DATA.x = (int)val;
    SetEvent(hUpdateEvtX);
}

void RWorker::setDataY(double val)
{
    RMutexLocker guard(hMutex);
    DATA.y = (int)val;
    SetEvent(hUpdateEvtY);
}



bool RWorker::isDataUpdated()
{
    if ((WaitForSingleObject(hUpdateEvtX, 0) == WAIT_OBJECT_0) &&
        (WaitForSingleObject(hUpdateEvtY, 0) == WAIT_OBJECT_0)) return true;
    else return false;
}


DWORD WINAPI RWorker::ThreadFuncX(LPVOID ptr)
{
    RWorker* _this = (RWorker*)ptr;
    if (!_this) return 1; //забыли нам дать объект, не будем работать

    for (;;)
    {
        if (WaitForSingleObject(_this->hUpdateEvtX, 0) == WAIT_TIMEOUT)
        {
            double x = _this->PARAMS.Ax * sin(_this->PARAMS.Wx * _this->PARAMS.t + _this->PARAMS.Dx);
            _this->PARAMS.t += 0.5;
            _this->setDataX(x);
        }
        //сделаем важную работу
                                             //_this->setData( _this->getData() + 1 );
        //поспим, глядя на флаг конца работы
        if (WaitForSingleObject(_this->hStopEvtX, 10) != WAIT_TIMEOUT)
            break;
    }
    return 0;
}

DWORD WINAPI RWorker::ThreadFuncY(LPVOID ptr)
{
    RWorker* _this = (RWorker*)ptr;
    if (!_this) return 1; //забыли нам дать объект, не будем работать

    for (;;)
    {
        if (WaitForSingleObject(_this->hUpdateEvtY, 0) == WAIT_TIMEOUT)
        {
            double y = _this->PARAMS.Ay * sin(_this->PARAMS.Wy * _this->PARAMS.t + _this->PARAMS.Dy);
            _this->PARAMS.t += 0.5;
            _this->setDataY(y);
        }
        //_this->setData(_this->getData() + 1);
        //поспим, глядя на флаг конца работы
        if (WaitForSingleObject(_this->hStopEvtY, 10) != WAIT_TIMEOUT)
            break;
    }
    return 0;
}



//---------------------------------------------------------------------------
#pragma package(smart_init)

