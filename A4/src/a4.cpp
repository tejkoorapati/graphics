#include "a4.hpp"


void antiAliasing(Image &img) {
    bool rightSafe;
    bool leftSafe;
    bool botSafe;
    bool topSafe;

    for(int y = 0 ; y < img.height(); y++) {
        for(int x = 0; x < img.width(); x ++) {

            rightSafe = false;
            leftSafe  = false;
            botSafe   = false;
            topSafe   = false;

            int count = 1;
            double R = img(x, y, 0);
            double G = img(x, y, 1);
            double B = img(x, y, 2);

            //left
            if(x - 1 >= 0) {
                R += img(x - 1, y, 0);
                G += img(x - 1, y, 1);
                B += img(x - 1, y, 2);
                count++;
                leftSafe = true;
            }

            //right
            if(x + 1 < img.width()) {
                R += img(x + 1, y, 0);
                G += img(x + 1, y, 1);
                B += img(x + 1, y, 2);
                count++;
                rightSafe = true;
            }

            //top
            if(y - 1 >= 0) {
                R += img(x, y - 1, 0);
                G += img(x, y - 1, 1);
                B += img(x, y - 1, 2);
                count++;
                topSafe = true;
            }

            //bot
            if(y + 1 < img.height()) {
                R += img(x, y + 1, 0);
                G += img(x, y + 1, 1);
                B += img(x, y + 1, 2);
                count++;
                botSafe = true;
            }

            if(topSafe && rightSafe) {
                R += img(x + 1, y - 1, 0);
                G += img(x + 1, y - 1, 1);
                B += img(x + 1, y - 1, 2);
                count++;
            }

            if(topSafe && leftSafe) {
                R += img(x - 1, y - 1, 0);
                G += img(x - 1, y - 1, 1);
                B += img(x - 1, y - 1, 2);
                count++;
            }

            if(botSafe && rightSafe) {
                R += img(x + 1, y + 1, 0);
                G += img(x + 1, y + 1, 1);
                B += img(x + 1, y + 1, 2);
                count++;
            }

            if(botSafe && leftSafe) {
                R += img(x - 1, y + 1, 0);
                G += img(x - 1, y + 1, 1);
                B += img(x - 1, y + 1, 2);
                count++;
            }

            R /= count;
            G /= count;
            B /= count;
            Colour finalColor(R, G, B);

            img(x, y, 0) = finalColor.R();
            img(x, y, 1) = finalColor.G();
            img(x, y, 2) = finalColor.B();
        }
    }
}


void a4_render(SceneNode* root,
               const std::string& filename,
               int width_i, int height_i,
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               const Colour& ambient,
               const std::list<Light*>& lights
              ) {
    // Fill in raytracing code here.
    double width = width_i;
    double height = height_i;
    Vector3D m_view = view;
    m_view.normalize();
    Vector3D m_up = up;
    m_up.normalize();
    Vector3D m_side = view.cross(up);
    m_side.normalize();
    Image img(width, height, 3);
    int count = 0;
    fov = fov * M_PI / 360;

    for(int y = 0; y < width; y++) {
        for(int x = 0; x < height; x++) {
            Ray ray;
            ray.origin = (eye);
            ray.direction = (x / width * 2 - 1) * tan(fov) * (width / height) * m_side + (y / height * 2 - 1) * tan(fov) * -m_up + m_view;
            ray.direction.normalize();
            Intersection intersection = root->calcIntersection(ray);
            Colour finalColor(0, 0, y / (height / 2));

            if(intersection.valid) {
                Vector3D intersectionDirection = -ray.direction;
                intersectionDirection.normalize();
                Vector3D intersectionNormal =  intersection.normal;
                intersectionNormal.normalize();
                Colour diffuse(0, 0, 0);
                Colour specular(0, 0, 0);
                Colour matDiff =  intersection.material->getDiffuse();
                double matShine =  intersection.material->getShininess();
                Colour matSpec =  intersection.material->getSpecular();
                Vector3D incidenceVector;
                Vector3D reflectedVector;

                for(std::list<Light*>::const_iterator it = lights.begin(); it != lights.end(); ++it) {
                    Light currLight = **it;
                    incidenceVector = currLight.position - intersection.location;
                    incidenceVector.normalize();
                    reflectedVector = 2 * incidenceVector.dot(intersectionNormal) * intersectionNormal - incidenceVector;
                    reflectedVector.normalize();
                    Ray shadowRay;
                    shadowRay.direction = (incidenceVector);
                    shadowRay.origin = (intersection.location);
                    Intersection shadowIntersection = root->calcIntersection(shadowRay);

                    if(!shadowIntersection.valid) {
                        double shininess = pow(reflectedVector.dot(intersectionDirection), matShine);

                        if((incidenceVector.dot(intersectionNormal)) > 0)
                            diffuse = diffuse + matDiff * incidenceVector.dot(intersectionNormal) * currLight.colour;

                        if(reflectedVector.dot(intersectionDirection) > 0)
                            specular = specular + matSpec * shininess * currLight.colour;
                    }
                }

                finalColor = (specular  + diffuse + ambient) * matDiff;
            }

            img(x, y, 0) = finalColor.R();
            img(x, y, 1) = finalColor.G();
            img(x, y, 2) = finalColor.B();
        }

        count++;

        if(count % 2 == 0)
            std::cerr << "Progress: " << (int)((y * 100) / height) << "% \n";
    }

    // Save the final file
    antiAliasing(img);
    img.savePng(filename);
    std::cout << "saved to file " << filename << std::endl;
}


double distanceBetween(Point3D a, Point3D b) {
    Point3D c ;
    c[0] = b[0] - a[0];
    c[1] = b[1] - a[1];
    c[2] = b[2] - a[2];
    return sqrt(pow(c[0], 2) + pow(c[1], 2) + pow(c[2], 2));

}


