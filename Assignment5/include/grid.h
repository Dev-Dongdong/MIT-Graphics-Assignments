/*
 * @Description: Grid Class derive from object3D
 * @Author: LvBujing
 * @Date: 2021-01-18 16:29:51
 * @LastEditTime: 2021-10-01 10:49:29
 * @LastEditors: LvBujing
 */
#ifndef _GRID_H_
#define _GRID_H_
#define EPSILON 0.00001f

#include "boundingbox.h"
#include "object3d.h"
#include "marchingInfo.h"
#include "rayTree.h"
#include "object3dvector.h"
#include "phongMaterial.h"

class Grid : public Object3D{

private:
    Object3DVector *cells;
    Vec3f nVector;  // Contain all three divided nums
    void DrawQuads(Vec3f a, Vec3f b, Vec3f c, Vec3f d, Material* mat){
        mat->glSetMaterial();

        Vec3f E0 = b - a;
        Vec3f E1 = d - a;
        Vec3f normal;
        Vec3f::Cross3(normal,E1,E0);
        normal.Normalize();
        //cout<<"here are quads: "<<a<<" "<<b<<" "<<c<<" "<<d<<endl;
        //cout<<"here are normal: "<<normal<<endl;
        
        glBegin(GL_QUADS);
            glNormal3f(-normal.x(),-normal.y(),-normal.z());
            glVertex3f(a.x(),a.y(),a.z());
            glVertex3f(b.x(),b.y(),b.z());
            glVertex3f(c.x(),c.y(),c.z());
            glVertex3f(d.x(),d.y(),d.z());
        glEnd();
        
    }
    
    // Check the pos is whether in a plane of the grid which is parallel with X plane.
    bool isInXPlane(Vec3f &pos){
        Vec3f minP = box->getMin();
        Vec3f maxP = box->getMax();
        float deltaX = (maxP.x() - minP.x()) / nVector.x();
        float x_pieces = (pos.x() - minP.x()) / deltaX;
        return abs(x_pieces - (int)(x_pieces + EPSILON)) < EPSILON; // Here plus EPSILON before transform to "int" is neccessary, due to float trouble.
    }

    // Check the pos is whether in a plane of the grid which is parallel with Y plane.
    bool isInYPlane(Vec3f &pos){
        Vec3f minP = box->getMin();
        Vec3f maxP = box->getMax();
        float deltaY = (maxP.y() - minP.y()) / nVector.y();
        float y_pieces = (pos.y() - minP.y()) / deltaY;
        return abs(y_pieces - (int)(y_pieces + EPSILON)) < EPSILON; // Here plus EPSILON before transform to "int" is neccessary, due to float trouble.
    }
    
    // Check the pos is whether in a plane of the grid which is parallel with Z plane.
    bool isInZPlane(Vec3f &pos){
        Vec3f minP = box->getMin();
        Vec3f maxP = box->getMax();
        float deltaZ = (maxP.z() - minP.z()) / nVector.z();
        float z_pieces = (pos.z() - minP.z()) / deltaZ;
        return abs(z_pieces - (int)(z_pieces + EPSILON)) < EPSILON; // Here plus EPSILON before transform to "int" is neccessary, due to float trouble.
    }
    // Find the next nearest t in x,y,z Axis.
    void getNextT(float &t_next_x, float &t_next_y,float &t_next_z, Vec3f &origin, Vec3f &dir,
                Vec3f pos, Vec3f &sign, float &deltaX, float &deltaY, float &deltaZ, Vec3f &minP){
        
        if(!isInXPlane(pos)){  // pos is not in a plane which is parallel with yz-plane
            if(sign.x() > 0)pos.Set((int)((pos.x() - minP.x()) / deltaX + EPSILON) * deltaX + minP.x(),pos.y(),pos.z());
            else pos.Set(((int)((pos.x() - minP.x()) / deltaX + EPSILON) - sign.x()) * deltaX + minP.x(),pos.y(),pos.z());
        }

        if(!isInYPlane(pos)){  // pos is not in a plane which is parallel with xz plane
            if(sign.y() > 0)pos.Set(pos.x(),(int)((pos.y() - minP.y()) / deltaY + EPSILON) * deltaY + minP.y(),pos.z());
            else pos.Set(pos.x(),((int)((pos.y() - minP.y()) / deltaY + EPSILON) - sign.y()) * deltaY + minP.y(),pos.z());
        }
        if(!isInZPlane(pos)){  // pos is not in a plane which is parallel with xy plane
            if(sign.z() > 0)pos.Set(pos.x(),pos.y(),((int)(pos.z() - minP.z()) / deltaZ + EPSILON) * deltaZ + minP.z());
            else pos.Set(pos.x(),pos.y(),((int)((pos.z() - minP.z()) / deltaZ + EPSILON) - sign.z()) * deltaZ + minP.z());
        }

        if(abs(sign.x()) < EPSILON)t_next_x = INFINITY;   // if the ray is perpendicular to X axis
        else t_next_x = (pos.x() + sign.x() * deltaX - origin.x()) / dir.x();
        if(abs(sign.y()) < EPSILON)t_next_y = INFINITY;   // if the ray is perpendicular to Y axis
        else t_next_y = (pos.y() + sign.y() * deltaY - origin.y()) / dir.y();
        if(abs(sign.z()) < EPSILON)t_next_z = INFINITY;   // if the ray is perpendicular to Z axis
        else t_next_z = (pos.z() + sign.z() * deltaZ - origin.z()) / dir.z();
    }
    void AddFacesToRayTree(const int &i, const int &j, const int &k, const Vec3f &enter_face_normal, const int &num){
        int nx = nVector.x(), ny = nVector.y(), nz = nVector.z();
        int length = nx * ny * nz;
        Vec3f minP = box->getMin();
        Vec3f maxP = box->getMax();
        float deltaX = (maxP.x() - minP.x()) / nx;
        float deltaY = (maxP.y() - minP.y()) / ny;
        float deltaZ = (maxP.z() - minP.z()) / nz;
        Material* paintMaterial = getColorGradient(num);
        Vec3f o = minP + Vec3f(i*deltaX,j*deltaY,k*deltaZ);
        RayTree::AddHitCellFace(o, o + Vec3f(0,deltaY,0), o + Vec3f(0,deltaY,deltaZ),o + Vec3f(0,0,deltaZ), Vec3f(1,0,0),paintMaterial);
        if(abs(enter_face_normal.x() + 1) < EPSILON)RayTree::AddEnteredFace(o, o + Vec3f(0,deltaY,0), o + Vec3f(0,deltaY,deltaZ),o + Vec3f(0,0,deltaZ), Vec3f(1,0,0),paintMaterial);

        RayTree::AddHitCellFace(o + Vec3f(deltaX,0,0),o + Vec3f(deltaX,0,deltaZ), o + Vec3f(deltaX,deltaY,deltaZ),o + Vec3f(deltaX,deltaY,0), Vec3f(-1,0,0),paintMaterial);
        if(abs(enter_face_normal.x() - 1) < EPSILON)RayTree::AddEnteredFace(o + Vec3f(deltaX,0,0),o + Vec3f(deltaX,0,deltaZ), o + Vec3f(deltaX,deltaY,deltaZ),o + Vec3f(deltaX,deltaY,0), Vec3f(-1,0,0),paintMaterial);

        RayTree::AddHitCellFace(o, o + Vec3f(0,0,deltaZ), o + Vec3f(deltaX,0,deltaZ),o + Vec3f(deltaX,0,0), Vec3f(0,1,0),paintMaterial);
        if(abs(enter_face_normal.y() + 1) < EPSILON)RayTree::AddEnteredFace(o, o + Vec3f(0,0,deltaZ), o + Vec3f(deltaX,0,deltaZ),o + Vec3f(deltaX,0,0), Vec3f(0,1,0),paintMaterial);

        RayTree::AddHitCellFace(o + Vec3f(0,deltaY,0),o + Vec3f(deltaX,deltaY,0), o + Vec3f(deltaX,deltaY,deltaZ),o + Vec3f(0,deltaY,deltaZ), Vec3f(0,-1,0),paintMaterial);
        if(abs(enter_face_normal.y() - 1) < EPSILON)RayTree::AddEnteredFace(o + Vec3f(0,deltaY,0),o + Vec3f(deltaX,deltaY,0), o + Vec3f(deltaX,deltaY,deltaZ),o + Vec3f(0,deltaY,deltaZ), Vec3f(0,-1,0),paintMaterial);

        RayTree::AddHitCellFace(o, o + Vec3f(deltaX,0,0), o + Vec3f(deltaX,deltaY,0),o + Vec3f(0,deltaY,0), Vec3f(0,0,1),paintMaterial);
        if(abs(enter_face_normal.z() + 1) < EPSILON)RayTree::AddEnteredFace(o, o + Vec3f(deltaX,0,0), o + Vec3f(deltaX,deltaY,0),o + Vec3f(0,deltaY,0), Vec3f(0,0,1),paintMaterial);

        RayTree::AddHitCellFace(o + Vec3f(0,0,deltaZ),o + Vec3f(0,deltaY,deltaZ), o + Vec3f(deltaX,deltaY,deltaZ),o + Vec3f(deltaX,0,deltaZ), Vec3f(0,0,-1),paintMaterial);
        if(abs(enter_face_normal.z() - 1) < EPSILON)RayTree::AddEnteredFace(o + Vec3f(0,0,deltaZ),o + Vec3f(0,deltaY,deltaZ), o + Vec3f(deltaX,deltaY,deltaZ),o + Vec3f(deltaX,0,deltaZ), Vec3f(0,0,-1),paintMaterial);
    }
public:
    // TODO: There is some mistakes with bool array.Fix it.
    Grid(BoundingBox *bb, int nx, int ny, int nz){ 
        box = bb; cells = new Object3DVector[nx * ny * nz]; 
        nVector = Vec3f(nx,ny,nz); }
    ~Grid(){ delete [] cells;}
    void setMat(Material *mat){ m = mat; }
    void setBoundingBox(BoundingBox* bb){ box = bb;}
    virtual bool intersect(const Ray &r, Hit &h, float tmin){
        MarchingInfo mi;
        bool isHit = false;
        initializeRayMarch(mi,r,tmin);
        if(mi.getGridIndexI() < 0 || mi.getGridIndexJ() < 0 || mi.getGridIndexK() < 0)return false;

        int indexI = mi.getGridIndexI(), indexJ = mi.getGridIndexJ(), indexK = mi.getGridIndexK();
        int index = (int)(indexI * nVector.y() * nVector.z() + indexJ * nVector.z() + indexK);
        int num = 1;
        //cout<<"Enter index is: "<<indexI<<" "<<indexJ<<" "<<indexK<<endl;
        while(indexI >= 0 && indexI < nVector.x() && indexJ >= 0 && indexJ < nVector.y() && indexK >= 0 && indexK < nVector.z()){
            if(!isHit && cells[index].getNumObjects() != 0){
                isHit = true;
                //cout<<"Now record I J K: "<<indexI<<" "<<indexJ<<" "<<indexK<<endl;
                //if(indexI == 0 && indexJ == 3 && indexK == 0)cout<<"indexI: "<<indexI<<" indexJ: "<<indexJ<<" indexK: "<<indexK<<endl;
                //cout<<mi.getNowT()<<" "<<mi.getNormal()<<endl;
                h.set(mi.getNowT(),getColorGradient(cells[index].getNumObjects()),mi.getNormal(),r); //for offline-rendering(like the colorful rabbit)
            }
            AddFacesToRayTree(indexI,indexJ,indexK,mi.getNormal(),num); //show this face colorfully
            num++;  //when adding faces, we give it color not depending on how many objects the grid cell has
            //mi.print();
            mi.nextCell();
            indexI = mi.getGridIndexI();
            indexJ = mi.getGridIndexJ();
            indexK = mi.getGridIndexK();
            index = (int)(indexI * nVector.y() * nVector.z() + indexJ * nVector.z() + indexK);
        }
        return isHit;
    }

    Vec3f getNVector(){
        return nVector;
    }
    int getNx(){ return nVector.x();}
    int getNy(){ return nVector.y();}
    int getNz(){ return nVector.z();}

    void addCellObject(const int &index, Object3D* obj){ 
        //cout<<"now add index: "<<index<<endl;
        assert(index >= 0 && index < nVector.x() * nVector.y() * nVector.z()); 
        cells[index].addObject(obj); }

    void paint(){
        int nx = nVector.x(), ny = nVector.y(), nz = nVector.z();
        int length = nx * ny * nz;
        Vec3f minP = box->getMin();
        Vec3f maxP = box->getMax();
        float deltaX = (maxP.x() - minP.x()) / nx;
        float deltaY = (maxP.y() - minP.y()) / ny;
        float deltaZ = (maxP.z() - minP.z()) / nz;
        // The coordinates to index is : "ny*nz*X + nz*Y + Z = index"
        Material* paintMaterial;
        for(int i = 0; i < nx; i++)
            for(int j = 0; j < ny; j++)
                for(int k = 0; k < nz; k++){
                    int index = ny*nz*i + nz*j + k;
                    int nums = cells[index].getNumObjects();
                    paintMaterial = getColorGradient(nums); // Give every cells unique colors according to its objects.
                    if(cells[index].getNumObjects() != 0){   // This should be opaque
                        //cout<<"paint index = "<<index<<endl;
                        Vec3f o = minP + Vec3f(i*deltaX,j*deltaY,k*deltaZ);
                        if(i - 1 < 0 || (index - ny*nz >= 0 && cells[index - ny * nz].getNumObjects() == 0)){   //X-1
                            DrawQuads(o, o + Vec3f(0,deltaY,0), o + Vec3f(0,deltaY,deltaZ),o + Vec3f(0,0,deltaZ), paintMaterial);
                        }
                        if(i + 1 >= nx || (index + ny*nz < length && cells[index + ny * nz].getNumObjects() == 0)){ // X+1
                            DrawQuads(o + Vec3f(deltaX,0,0),o + Vec3f(deltaX,0,deltaZ), o + Vec3f(deltaX,deltaY,deltaZ),o + Vec3f(deltaX,deltaY,0), paintMaterial);
                        }
                        if(j - 1 < 0 || (index - nz >= 0 && cells[index - nz].getNumObjects() == 0)){   // Y-1
                            DrawQuads(o, o + Vec3f(0,0,deltaZ), o + Vec3f(deltaX,0,deltaZ),o + Vec3f(deltaX,0,0), paintMaterial);
                        }
                        if(j + 1 >= ny || (index + nz < length && cells[index + nz].getNumObjects() == 0)){ // Y+1
                            DrawQuads(o + Vec3f(0,deltaY,0),o + Vec3f(deltaX,deltaY,0), o + Vec3f(deltaX,deltaY,deltaZ),o + Vec3f(0,deltaY,deltaZ), paintMaterial);
                        }
                        if(k - 1 < 0 || (index - 1 >= 0 && cells[index - 1].getNumObjects() == 0)){ // Z-1
                            DrawQuads(o, o + Vec3f(deltaX,0,0), o + Vec3f(deltaX,deltaY,0),o + Vec3f(0,deltaY,0), paintMaterial);
                        }
                        if(k + 1 >= nz || (index + 1 < length && cells[index + 1].getNumObjects() == 0)){   //  Z+1
                            DrawQuads(o + Vec3f(0,0,deltaZ),o + Vec3f(0,deltaY,deltaZ), o + Vec3f(deltaX,deltaY,deltaZ),o + Vec3f(deltaX,0,deltaZ), paintMaterial);
                        }
                    }
                }
    }

    void Print(){
        assert(cells != NULL);
        int length = nVector.x() * nVector.y() * nVector.z();
        cout<<"#GRID LENGTH : "<<nVector.x() * nVector.y() * nVector.z()<<endl;
        /*
        cout<<"#GRID CONTENTS#"<<endl;
        for(int i = 0; i < length; i++){
            cout<<cells[i]<<" ";
        }
        cout<<endl;
        */
    }
    void insertIntoGrid(Grid *g, Matrix *m){
        
    }

    // Calculate marching increments and information for the first cell
    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) {
        int i,j,k;  //  current grid cell index
        Vec3f t_next,d_t,sign,normal;   // t_next: next values of intersection along each axis
                                        // d_t: marching increments along each axis
                                        // sign: sign of the direction vector components
                                        // normal: the normal of face which was crossed
        // Variables initialization
        Vec3f origin = r.getOrigin(), dir = r.getDirection();
        float sign_x,sign_y,sign_z;
        if(abs(dir.x()) < EPSILON)sign_x = 0;
        else sign_x = (dir.x() > 0) ? 1 : -1;
        if(abs(dir.y()) < EPSILON)sign_y = 0;
        else sign_y = (dir.y() > 0) ? 1 : -1;
        if(abs(dir.z()) < EPSILON)sign_z = 0;
        else sign_z = (dir.z() > 0) ? 1 : -1;
        sign.Set(sign_x,sign_y,sign_z);
        
        // Ray-Box intersection
        Vec3f minP = box->getMin();
        Vec3f maxP = box->getMax();
        int nx = nVector.x(), ny = nVector.y(), nz = nVector.z();
        float deltaX = (maxP.x() - minP.x()) / nx;
        float deltaY = (maxP.y() - minP.y()) / ny;
        float deltaZ = (maxP.z() - minP.z()) / nz;
        d_t.Set(abs(deltaX / dir.x()),abs(deltaY / dir.y()),abs(deltaZ / dir.z()));
        Vec3f tMin = Vec3f(min((minP.x() - origin.x()) / dir.x(),(maxP.x() - origin.x()) / dir.x()),
                           min((minP.y() - origin.y()) / dir.y(),(maxP.y() - origin.y()) / dir.y()),
                           min((minP.z() - origin.z()) / dir.z(),(maxP.z() - origin.z()) / dir.z()));
        Vec3f tMax = Vec3f(max((minP.x() - origin.x()) / dir.x(),(maxP.x() - origin.x()) / dir.x()),
                           max((minP.y() - origin.y()) / dir.y(),(maxP.y() - origin.y()) / dir.y()),
                           max((minP.z() - origin.z()) / dir.z(),(maxP.z() - origin.z()) / dir.z()));
        float tEnter = max(tMin.x(),max(tMin.y(),tMin.z()));    // Enter point's t for ray
        float tExit = min(tMax.x(),min(tMax.y(),tMax.z()));     // Exit point's t for ray
        if(tExit < tEnter || tExit < tmin) {    // The ray don't hit the box
            return;
        }
        
        // Get the first intersection of the box along the ray dir 
        Vec3f pos;
        float nowT;
        if(tExit > 0 && tEnter < 0) {   // The ray origin is in the box
            float inner_t_x,inner_t_y,inner_t_z;
            pos = origin;
            getNextT(inner_t_x,inner_t_y,inner_t_z,origin,dir,pos,sign,deltaX,deltaY,deltaZ,minP);
            float t_temp = min(inner_t_x,min(inner_t_y,inner_t_z));
            pos = origin + t_temp * dir;
            nowT = t_temp;
        } else {    // The ray origin is out of the box
            pos = origin + tEnter * dir;
            nowT = tEnter;
        }
        //cout<<"pos is: "<<pos<<endl;
        i = (pos.x() - minP.x() + EPSILON*sign.x()) / deltaX; j = (pos.y() - minP.y() + EPSILON*sign.y()) / deltaY; k = (pos.z() - minP.z() + EPSILON*sign.z()) / deltaZ;    // cell index
        //cout<<"i: "<<i<<" j: "<<j<<" k: "<<k<<endl;
        float t_next_x,t_next_y,t_next_z;
        getNextT(t_next_x,t_next_y,t_next_z,origin,dir,pos,sign,deltaX,deltaY,deltaZ,minP);
        t_next.Set(t_next_x, t_next_y, t_next_z);

        
        // Set normal
        if (isInXPlane(pos))normal = Vec3f(-sign.x(),0,0);
        else if(isInYPlane(pos))normal = Vec3f(0,-sign.y(),0);
        else if(isInZPlane(pos))normal = Vec3f(0,0,-sign.z());
        // Update mi
        mi.set(tmin,i,j,k,t_next,d_t,sign,normal,nowT);
        //cout<<pos<<" "<<normal<<endl;
    }

    // Return unique color according to numbers order.
    Material* getColorGradient(const int &nums){
        Vec3f diffuseColor(1,1,1);
        Vec3f specularColor(0,0,0);
        float exponent = 1;
        Vec3f reflectiveColor(0,0,0);
        Vec3f transparentColor(0,0,0);
        float indexOfRefraction = 1;
        switch(nums){
            case 0: diffuseColor = Vec3f(255.0f,255.0f,255.0f); break;
            case 1: diffuseColor = Vec3f(255.0f,255.0f,255.0f); break;
            case 2: diffuseColor = Vec3f(255.0f,000.0f,255.0f); break;
            case 3: diffuseColor = Vec3f(178.0f,000.0f,255.0f); break;
            case 4: diffuseColor = Vec3f(076.0f,000.0f,255.0f); break;
            case 5: diffuseColor = Vec3f(000.0f,000.0f,255.0f); break;
            case 6: diffuseColor = Vec3f(000.0f,076.0f,255.0f); break;
            case 7: diffuseColor = Vec3f(000.0f,178.0f,255.0f); break;
            case 8: diffuseColor = Vec3f(000.0f,255.0f,255.0f); break;
            case 9: diffuseColor = Vec3f(000.0f,255.0f,178.0f); break;
            case 10: diffuseColor = Vec3f(000.0f,255.0f,076.0f); break;
            case 11: diffuseColor = Vec3f(000.0f,255.0f,000.0f); break;
            case 12: diffuseColor = Vec3f(076.0f,255.0f,000.0f); break;
            case 13: diffuseColor = Vec3f(178.0f,255.0f,000.0f); break;
            case 14: diffuseColor = Vec3f(255.0f,255.0f,000.0f); break;
            case 15: diffuseColor = Vec3f(255.0f,178.0f,000.0f); break;
            case 16: diffuseColor = Vec3f(255.0f,076.0f,000.0f); break;
            default: diffuseColor = Vec3f(255.0f,000.0f,000.0f); break;
        }
        diffuseColor /= 255.0f;
        Material *answer = new PhongMaterial(diffuseColor,specularColor,exponent,
				       reflectiveColor,transparentColor,
				       indexOfRefraction);
        return answer;
    }
};


#endif
