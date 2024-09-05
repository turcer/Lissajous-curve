//---------------------------------------------------------------------------

#ifndef workerH
#define workerH
#include <windows.h>
#include <math.h>

//Вспомогательный класс, очень помогает защищать данные.
//Защищает данные буквально до самой смерти
class RMutexLocker
{
    HANDLE hMutex;
public:
    RMutexLocker(HANDLE mutex)
        :hMutex(mutex)
    {
        WaitForSingleObject(hMutex, INFINITE);
    }
    ~RMutexLocker()
    {
        ReleaseMutex(hMutex);
    }
};

class RWorker
{
    HANDLE hUpdateEvtX; //событие "данные обновились"
    HANDLE hUpdateEvtY;
    HANDLE hStopEvtX;   //событие "конец работы"
    HANDLE hStopEvtY;

    HANDLE hMutex;  //защита данных




    HANDLE hThread1; //дескриптор потока
    HANDLE hThread2; //дескриптор потока
public:

    struct coordinates//-------------------------------------------------
    {
        int x;
        int y;
    };
    coordinates DATA;//очень важные данные (были peivate)----------------

    RWorker();
    ~RWorker();

    struct parameters
    {
        int Ax, Wx, Dx;
        int Ay, Wy, Dy;
        int t;
    };
    parameters PARAMS;

    class myinput
    {
    public:
        parameters Params;
    };
    myinput input;


    void setParams(parameters val);

    void increaseTime();

    int     getDataX();
    int     getDataY();
    void    setDataX(double val);
    void    setDataY(double val);
    bool    isDataUpdated();

    void    startWorkX();
    void    startWorkY();
    void    stopWorkX();
    void    stopWorkY();

private:
    static DWORD WINAPI ThreadFuncX(LPVOID ptr);
    static DWORD WINAPI ThreadFuncY(LPVOID ptr);
};


//---------------------------------------------------------------------------
#endif
