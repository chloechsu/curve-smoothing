#include <iostream>
using namespace std;
int main( int argc, char *argv[] )
{
    char c;
    int vertexcount = 0, index;
    double x,y,z;
    double rx = 0.5;
    double ry = 0.5;
    while(true)
    {
        do{
            cin>>c;
        }while(c!='v' && c!='f');
        if(c == 'v')
        {
            cin>>x>>y>>z;
            vertexcount++;
            //double rx = 0.01 * ((double)rand() / (double)(RAND_MAX) - 0.5);
            //double ry = 0.01 * ((double)rand() / (double)(RAND_MAX) - 0.5);
            cout<<"v "<<x+rx<<" "<<y+ry<<" "<<z<<endl;
            rx = -rx;
            ry = -ry;
        }
        if(c == 'f')
        {
            cout<<"f ";
            for(int i=0;i<vertexcount;i++)
            {
                cin>>index;
                cout<<index<<" ";
            }
            break;
        }
    }
}
