
#include "a2.hpp"

void rotate(Matrix4x4 &mat, double angle, int axis, int mode)
{
    Matrix4x4 temp;

    Vector4D r1,r2,r3,r4;
    double c=cos(angle);
    double s=sin(angle);

    switch(axis){

        case 1:
        r1=Vector4D(1,0,0,0);
        r2=Vector4D(0,c,-s,0);
        r3=Vector4D(0,s,c,0);
        r4=Vector4D(0,0,0,1);
        break;
        case 2:
        r1=Vector4D(c,0,s,0);
        r2=Vector4D(0,1,0,0);
        r3=Vector4D(-s,0,c,0);
        r4=Vector4D(0,0,0,1);
        break;
        case 3 :
        r1=Vector4D(c,-s,0,0);
        r2=Vector4D(s,c,0,0);
        r3=Vector4D(0,0,1,0);
        r4=Vector4D(0,0,0,1);
        break;
    }

    temp=Matrix4x4(r1,r2,r3,r4);
    if(mode == 1){
        mat= temp * mat ;
    } else{
        mat = mat * temp;
    }
}


void translate(Matrix4x4 &mat,double x,double y,double z)
{
    Matrix4x4 temp;
    Vector4D r1,r2,r3,r4;

    r1=Vector4D(1,0,0,x);
    r2=Vector4D(0,1,0,y);
    r3=Vector4D(0,0,1,z);
    r4=Vector4D(0,0,0,1);
    temp=Matrix4x4(r1,r2,r3,r4);
    mat=mat*temp;

}

void scale(Matrix4x4 &mat,double x,double y,double z)
{
    Matrix4x4 temp;
    Vector4D r1,r2,r3,r4;

    r1=Vector4D(x,0,0,0);
    r2=Vector4D(0,y,0,0);
    r3=Vector4D(0,0,z,0);
    r4=Vector4D(0,0,0,1);
    temp=Matrix4x4(r1,r2,r3,r4);
    mat=temp*mat;

}