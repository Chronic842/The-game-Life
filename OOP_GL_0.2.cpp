#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <conio.h>
#include <stdlib.h>
#include "glut.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")

using namespace std;

string fname = "drp1.txt";

class GRID {
private:
    int GridSize;
    vector <vector <int>> arr;
    vector <vector <int>> arr1;
public:
    void Set_GridSize(int);
    int Get_GridSize();
    void ReadFile();
    void DrawGrid();
    void Iter();
};

void GRID::Set_GridSize(int _GridSize)
{
    GridSize = _GridSize;
}

int GRID::Get_GridSize()
{
    return GridSize;
}

void GRID::ReadFile()
{
    int cnt = 0;
    ifstream FILE(fname);
    if (FILE.is_open())
    {
        string temp;
        getline(FILE, temp);
        stringstream num;
        num << temp;
        while (!num.eof())
        {
            num >> temp;
            cnt += 1;
        }
    }
    else
    {
        cout << "Wrong file" << endl;
        exit(1);
    }
    Set_GridSize(cnt);

    FILE.clear();
    FILE.seekg(0);

    if (FILE.is_open())
    {
        for (int i = 0; i < Get_GridSize(); i++)
        {
            vector <int> t_arr;
            for (int j = 0; j < Get_GridSize(); j++)
            {
                int x;
                FILE >> x;
                t_arr.push_back(x);
                if (FILE.eof())
                    break;
            }
            arr.push_back(t_arr);
            arr1.push_back(t_arr);
        }
        FILE.close();
    }
    else
    {
        cout << "Wrong file" << endl;
        exit(1);
    }

    for (int i = 0; i < arr.size(); i++)
    {
        for (int j = 0; j < arr.size(); j++)
        {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
}

void GRID::DrawGrid()
{
    double NormXY = 2.0 / Get_GridSize();

    for (int i = 0; i < arr1.size(); i++)
    {
        double YTrans = 1 - (NormXY) - (NormXY * (double)i);
        for (int j = 0; j < arr1.size(); j++)
        {
            double XTrans = -1 + (NormXY) + (NormXY * (double)j);

            if (arr1[i][j] == 0)
            {
                glLoadIdentity();
                glTranslatef(XTrans, YTrans, 0);
                glColor3f(1, 1, 1);
                glBegin(GL_QUADS);
                glVertex3f(-NormXY, NormXY, 0);
                glVertex3f(NormXY, NormXY, 0);
                glVertex3f(NormXY, -NormXY, 0);
                glVertex3f(-NormXY, -NormXY, 0);
                glEnd();
            }
            else
            {
                glLoadIdentity();
                glTranslatef(XTrans, YTrans, 0);
                glColor3f(0, 0, 0);
                glBegin(GL_QUADS);
                glVertex3f(-NormXY, NormXY, 0);
                glVertex3f(NormXY, NormXY, 0);
                glVertex3f(NormXY, -NormXY, 0);
                glVertex3f(-NormXY, -NormXY, 0);
                glEnd();
            }
        }
    }

    double x = -1, y = -1;

    while (x <= 1)
    {
        glLoadIdentity();
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);
        glVertex3f(x, 1, 0);
        glVertex3f(x, -1, 0);
        glVertex3f(-1, x, 0);
        glVertex3f(1, x, 0);
        glEnd();
        x += NormXY;
    }
}

void GRID::Iter()
{
    for (int i = 0; i < arr.size(); i++)
    {
        for (int j = 0; j < arr.size(); j++)
        {
            int nb = 0;
            if (i > 0 && arr[i - 1][j] == 1) { nb++; }
            if (i < arr.size() - 1 && arr[i + 1][j] == 1) { nb++; }
            if (j > 0 && arr[i][j - 1] == 1) { nb++; }
            if (j < arr.size() - 1 && arr[i][j + 1] == 1) { nb++; }
            if (i > 0 && j > 0 && arr[i - 1][j - 1] == 1) { nb++; }
            if (i < arr.size() - 1 && j < arr.size() - 1 && arr[i + 1][j + 1] == 1) { nb++; }
            if (i > 0 && j < arr.size() - 1 && arr[i - 1][j + 1] == 1) { nb++; }
            if (i < arr.size() - 1 && j > 0 && arr[i + 1][j - 1] == 1) { nb++; }
            if (arr[i][j] == 1)
            {
                if (nb == 2 || nb == 3) { arr1[i][j] == 1; }
                else { arr1[i][j] = 0; };
            }
            else
            {
                if (nb == 3) { arr1[i][j] = 1; }
                else { arr1[i][j] == 0; };
            }
        }
    }

    for (int i = 0; i < arr.size(); i++)
    {
        for (int j = 0; j < arr.size(); j++)
        {
            arr[i][j] = arr1[i][j];
        }
    }
}

GRID GridObj;

void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    GridObj.DrawGrid();

    glFlush();
}

void Read_Keyboard(unsigned char k, int, int)
{
    if (k == 27)
    {
        cout << "Shutdown complete." << endl;
        exit(0);
    }
    if (k == 'n' || k == 'N')
    {
        GridObj.Iter();
        GridObj.DrawGrid();
        glutPostRedisplay();
    }
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_ALPHA);
    glutInitWindowPosition(600, 75);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Пример вывода 2D графики");
    glClearColor(1, 0, 0, 1);

    setlocale(LC_ALL, "Russian");
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(Read_Keyboard);
    
    GridObj.ReadFile();

    glutMainLoop();
    return 0;
}