//---------------------------------------------------------------------------

#ifndef workerH
#define workerH
#include <windows.h>
#include <math.h>

//��������������� �����, ����� �������� �������� ������.
//�������� ������ ��������� �� ����� ������
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
    HANDLE hUpdateEvtX; //������� "������ ����������"
    HANDLE hUpdateEvtY;
    HANDLE hStopEvtX;   //������� "����� ������"
    HANDLE hStopEvtY;

    HANDLE hMutex;  //������ ������




    HANDLE hThread1; //���������� ������
    HANDLE hThread2; //���������� ������
public:

    struct coordinates//-------------------------------------------------
    {
        int x;
        int y;
    };
    coordinates DATA;//����� ������ ������ (���� peivate)----------------

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

