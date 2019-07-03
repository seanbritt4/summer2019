#include "SphereWorld.h"

SphereWorld::SphereWorld(GLint latDiv, GLint lonDiv, GLfloat r)
{
    /*
    GLfloat twopi = PI*2;

    SphereWorldTile* t[latDivisions*lonDivisions];
    for(int i = 0; i < lonDivisions; i++){
        for(int j = 0; j < latDivisions; j++){
            t[(latDivisions*i)+j] = new SphereWorldTile(j*(PI/(latDivisions/2.0)), i*(PI/(lonDivisions/2)), 4, twopi/GLfloat(latDivisions),twopi/GLfloat(lonDivisions*2.0),glm::vec4(0,1,0,1));
            tiles.push_back(t[(latDivisions*i)+j]);
        }
        std::cout << i << std::endl;
    }
    */

    radius = r;
    numTiles = latDiv * lonDiv;
    latDivisions = latDiv;
    lonDivisions = lonDiv;

    vertex_vbo = 0;
    color_vbo = 0;
    normal_vbo = 0;
    glGenBuffers(1, &vertex_vbo);
    glGenBuffers(1, &color_vbo);
    glGenBuffers(1, &normal_vbo);

    vertex_vbo_border = 0;
    color_vbo_border = 0;
    normal_vbo_border = 0;
    glGenBuffers(1, &vertex_vbo_border);
    glGenBuffers(1, &color_vbo_border);
    glGenBuffers(1, &normal_vbo_border);

    vao = 0;
    borderVao = 0;
    glGenVertexArrays(1, &vao);
    glGenVertexArrays(1, &borderVao);

    std::cout << "Loading" << std::endl;
    LoadDataToGraphicsCard();
    std::cout << "Done loading" << std::endl;
}

SphereWorld::~SphereWorld()
{

}

void SphereWorld::LoadDataToGraphicsCard()
{
    //Set up some initial variables
    GLuint vPosition = 0;
    GLuint vColor = 1;
    GLuint vNormal = 2;

    GLfloat twopi = PI*2;

    GLfloat Ptheta;
    GLfloat Ppsi;
    /*
    GLfloat objectPsi = twopi/GLfloat(latDivisions);
    GLfloat objectTheta = twopi/GLfloat(lonDivisions*2.0);
    //*/
    GLfloat objectPsi = twopi/GLfloat(latDivisions);
    GLfloat objectTheta = twopi/GLfloat(lonDivisions*2.0);

    glm::vec3 topLeftPos, topRightPos, bottomLeftPos, bottomRightPos;
    glm::vec3 anchor = glm::vec3(0, 0, 0);
    GLfloat xn, yn, zn;


    //Populate the arrays, and load into VBOs
    GLfloat *vertices = new GLfloat[numTiles*18*3];
    GLfloat *normals = new GLfloat[numTiles*18*3];
    GLfloat *border_vertices = new GLfloat[numTiles*8*3];
    GLfloat *border_normals = new GLfloat[numTiles*8*3];

    GLuint lonStride = 54 * latDivisions;
    GLuint borderStride = 24 * latDivisions;

    GLfloat r = 4;
    for(unsigned int i = 0; i < lonDivisions; i++){
        for(unsigned int j = 0; j < latDivisions; j++){
/*
            Ppsi = j*(PI/(GLfloat(latDivisions/2.0)));
            Ptheta = i*(PI/(GLfloat(lonDivisions/2.0)));

            topLeftPos = glm::vec3(radius*sin(Ppsi)*cos(Ptheta),radius*sin(Ptheta),radius*cos(Ppsi)*cos(Ptheta));
            topRightPos = glm::vec3(radius*sin(Ppsi+objectPsi)*cos(Ptheta),radius*sin(Ptheta),radius*cos(Ppsi+objectPsi)*cos(Ptheta));
            bottomLeftPos = glm::vec3(radius*sin(Ppsi)*cos(Ptheta+objectTheta),radius*sin(Ptheta+objectTheta),radius*cos(Ppsi)*cos(Ptheta+objectTheta));
            bottomRightPos = glm::vec3(radius*sin(Ppsi+objectPsi)*cos(Ptheta+objectTheta),radius*sin(Ptheta+objectTheta),radius*cos(Ppsi+objectPsi)*cos(Ptheta+objectTheta));
//*/
//*
            Ppsi = j*(twopi/(GLfloat(latDivisions)));
            Ptheta = i*(PI/(GLfloat(lonDivisions)));

            topLeftPos = glm::vec3(radius*sin(Ppsi)*sin(Ptheta),radius*cos(Ptheta)*-1,radius*cos(Ppsi)*sin(Ptheta));
            topRightPos = glm::vec3(radius*sin(Ppsi+objectPsi)*sin(Ptheta),radius*cos(Ptheta)*-1,radius*cos(Ppsi+objectPsi)*sin(Ptheta));
            bottomLeftPos = glm::vec3(radius*sin(Ppsi)*sin(Ptheta+objectTheta),radius*cos(Ptheta+objectTheta)*-1,radius*cos(Ppsi)*sin(Ptheta+objectTheta));
            bottomRightPos = glm::vec3(radius*sin(Ppsi+objectPsi)*sin(Ptheta+objectTheta),radius*cos(Ptheta+objectTheta)*-1,radius*cos(Ppsi+objectPsi)*sin(Ptheta+objectTheta));
//*/
            //Upper Right of the Tile Face
            vertices[(i*lonStride)+(j*54)+0] = topLeftPos.x;
            vertices[(i*lonStride)+(j*54)+1] = topLeftPos.y;
            vertices[(i*lonStride)+(j*54)+2] = topLeftPos.z;
            vertices[(i*lonStride)+(j*54)+3] = topRightPos.x;
            vertices[(i*lonStride)+(j*54)+4] = topRightPos.y;
            vertices[(i*lonStride)+(j*54)+5] = topRightPos.z;
            vertices[(i*lonStride)+(j*54)+6] = bottomRightPos.x;
            vertices[(i*lonStride)+(j*54)+7] = bottomRightPos.y;
            vertices[(i*lonStride)+(j*54)+8] = bottomRightPos.z;

            //Lower Left of Tile Face
            vertices[(i*lonStride)+(j*54)+9] = topLeftPos.x;
            vertices[(i*lonStride)+(j*54)+10] = topLeftPos.y;
            vertices[(i*lonStride)+(j*54)+11] = topLeftPos.z;
            vertices[(i*lonStride)+(j*54)+12] = bottomRightPos.x;
            vertices[(i*lonStride)+(j*54)+13] = bottomRightPos.y;
            vertices[(i*lonStride)+(j*54)+14] = bottomRightPos.z;
            vertices[(i*lonStride)+(j*54)+15] = bottomLeftPos.x;
            vertices[(i*lonStride)+(j*54)+16] = bottomLeftPos.y;
            vertices[(i*lonStride)+(j*54)+17] = bottomLeftPos.z;

            //Upper anchor
            vertices[(i*lonStride)+(j*54)+18] = anchor.x;
            vertices[(i*lonStride)+(j*54)+19] = anchor.y;
            vertices[(i*lonStride)+(j*54)+20] = anchor.z;
            vertices[(i*lonStride)+(j*54)+21] = topLeftPos.x;
            vertices[(i*lonStride)+(j*54)+22] = topLeftPos.y;
            vertices[(i*lonStride)+(j*54)+23] = topLeftPos.z;
            vertices[(i*lonStride)+(j*54)+24] = topRightPos.x;
            vertices[(i*lonStride)+(j*54)+25] = topRightPos.y;
            vertices[(i*lonStride)+(j*54)+26] = topRightPos.z;

            //Right anchor
            vertices[(i*lonStride)+(j*54)+27] = anchor.x;
            vertices[(i*lonStride)+(j*54)+28] = anchor.y;
            vertices[(i*lonStride)+(j*54)+29] = anchor.z;
            vertices[(i*lonStride)+(j*54)+30] = topRightPos.x;
            vertices[(i*lonStride)+(j*54)+31] = topRightPos.y;
            vertices[(i*lonStride)+(j*54)+32] = topRightPos.z;
            vertices[(i*lonStride)+(j*54)+33] = bottomRightPos.x;
            vertices[(i*lonStride)+(j*54)+34] = bottomRightPos.y;
            vertices[(i*lonStride)+(j*54)+35] = bottomRightPos.z;

            //Lower anchor
            vertices[(i*lonStride)+(j*54)+36] = anchor.x;
            vertices[(i*lonStride)+(j*54)+37] = anchor.y;
            vertices[(i*lonStride)+(j*54)+38] = anchor.z;
            vertices[(i*lonStride)+(j*54)+39] = bottomRightPos.x;
            vertices[(i*lonStride)+(j*54)+40] = bottomRightPos.y;
            vertices[(i*lonStride)+(j*54)+41] = bottomRightPos.z;
            vertices[(i*lonStride)+(j*54)+42] = bottomLeftPos.x;
            vertices[(i*lonStride)+(j*54)+43] = bottomLeftPos.y;
            vertices[(i*lonStride)+(j*54)+44] = bottomLeftPos.z;

            //Left anchor
            vertices[(i*lonStride)+(j*54)+45] = anchor.x;
            vertices[(i*lonStride)+(j*54)+46] = anchor.y;
            vertices[(i*lonStride)+(j*54)+47] = anchor.z;
            vertices[(i*lonStride)+(j*54)+48] = bottomLeftPos.x;
            vertices[(i*lonStride)+(j*54)+49] = bottomLeftPos.y;
            vertices[(i*lonStride)+(j*54)+50] = bottomLeftPos.z;
            vertices[(i*lonStride)+(j*54)+51] = topLeftPos.x;
            vertices[(i*lonStride)+(j*54)+52] = topLeftPos.y;
            vertices[(i*lonStride)+(j*54)+53] = topLeftPos.z;

            border_vertices[(i*borderStride)+(j*24)+0] = topLeftPos.x;
            border_vertices[(i*borderStride)+(j*24)+1] = topLeftPos.y;
            border_vertices[(i*borderStride)+(j*24)+2] = topLeftPos.z;
            border_vertices[(i*borderStride)+(j*24)+3] = topRightPos.x;
            border_vertices[(i*borderStride)+(j*24)+4] = topRightPos.y;
            border_vertices[(i*borderStride)+(j*24)+5] = topRightPos.z;

            border_vertices[(i*borderStride)+(j*24)+6] = topRightPos.x;
            border_vertices[(i*borderStride)+(j*24)+7] = topRightPos.y;
            border_vertices[(i*borderStride)+(j*24)+8] = topRightPos.z;
            border_vertices[(i*borderStride)+(j*24)+9] = bottomRightPos.x;
            border_vertices[(i*borderStride)+(j*24)+10] = bottomRightPos.y;
            border_vertices[(i*borderStride)+(j*24)+11] = bottomRightPos.z;

            border_vertices[(i*borderStride)+(j*24)+12] = bottomRightPos.x;
            border_vertices[(i*borderStride)+(j*24)+13] = bottomRightPos.y;
            border_vertices[(i*borderStride)+(j*24)+14] = bottomRightPos.z;
            border_vertices[(i*borderStride)+(j*24)+15] = bottomLeftPos.x;
            border_vertices[(i*borderStride)+(j*24)+16] = bottomLeftPos.y;
            border_vertices[(i*borderStride)+(j*24)+17] = bottomLeftPos.z;

            border_vertices[(i*borderStride)+(j*24)+18] = bottomLeftPos.x;
            border_vertices[(i*borderStride)+(j*24)+19] = bottomLeftPos.y;
            border_vertices[(i*borderStride)+(j*24)+20] = bottomLeftPos.z;
            border_vertices[(i*borderStride)+(j*24)+21] = topLeftPos.x;
            border_vertices[(i*borderStride)+(j*24)+22] = topLeftPos.y;
            border_vertices[(i*borderStride)+(j*24)+23] = topLeftPos.z;

            xn = (topLeftPos.x + topRightPos.x + bottomRightPos.x + bottomLeftPos.x)/4.0;
            yn = (topLeftPos.y + topRightPos.y + bottomLeftPos.y + bottomRightPos.y)/4.0;
            zn = (topLeftPos.z + topRightPos.z + bottomLeftPos.z + bottomRightPos.z)/4.0;

            for(int k = 0; k < 18; k++)
            {
                normals[(i*lonStride)+(j*54)+(k*3)+0] = xn;
                normals[(i*lonStride)+(j*54)+(k*3)+1] = yn;
                normals[(i*lonStride)+(j*54)+(k*3)+2] = zn;
            }

            for(int k = 0; k < 4; k++)
            {
                border_normals[(i*borderStride)+(j*24)+(k*3)+0] = xn;
                border_normals[(i*borderStride)+(j*24)+(k*3)+1] = yn;
                border_normals[(i*borderStride)+(j*24)+(k*3)+2] = zn;
            }

        }
    }

    //Color of the vertexes
    GLfloat *vertex_colors = new GLfloat[numTiles*18*3];
    for(unsigned int i = 0; i < numTiles*18; i++)
    {
        vertex_colors[i*3+0] = 0.0;
        vertex_colors[i*3+1] = 1.0;
        vertex_colors[i*3+2] = 0.0;
    }

    //Color of the borders between tiles
    GLfloat *border_colors = new GLfloat[numTiles*24];
    for(unsigned int i = 0; i < numTiles*24; i++)
        border_colors[i] = 1.0;

    //Load vertices into vertex_vbo
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, numTiles*18*3*sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);

    //Load vertex_colors into color_vbo
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, numTiles*18*3*sizeof(GLfloat), vertex_colors, GL_DYNAMIC_DRAW);

    //Load normals into normal_vbo
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
    glBufferData(GL_ARRAY_BUFFER, numTiles*18*3*sizeof(GLfloat), normals, GL_DYNAMIC_DRAW);

    //Load border_vertices into vertex_vbo_border
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_border);
    glBufferData(GL_ARRAY_BUFFER, numTiles*8*3*sizeof(GLfloat), border_vertices, GL_DYNAMIC_DRAW);

    //Load border_colors into color_vbo_border
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo_border);
    glBufferData(GL_ARRAY_BUFFER, numTiles*8*3*sizeof(GLfloat), border_colors, GL_DYNAMIC_DRAW);

    //Load border_normals into normal_vbo_border
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_border);
    glBufferData(GL_ARRAY_BUFFER, numTiles*8*3*sizeof(GLfloat), border_normals, GL_DYNAMIC_DRAW);

    //Define VAO
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);

    glBindVertexArray(borderVao);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_border);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_border);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo_border);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);

    delete[] vertices;
    delete[] vertex_colors;
    delete[] normals;
    delete[] border_vertices;
    delete[] border_colors;
    delete[] border_normals;

}

GLboolean SphereWorld::changeColorArray(glm::vec3 *c)
{
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    GLfloat *ptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if(ptr){
        for(unsigned int i = 0; i < numTiles; i++)
        {
            for(unsigned int vertex = 0; vertex < 18; vertex++)
            {
                    *ptr = c->r;
                    ptr++;
                    *ptr = c->g;
                    ptr++;
                    *ptr = c->b;
                    ptr++;
            }
            c++;
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return true;
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return false;
    }
}

GLboolean SphereWorld::changeColor(GLuint lat, GLuint lon, glm::vec3 c)
{
    if(lon > lonDivisions || lat > latDivisions)
        return false;

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    GLfloat *ptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if(ptr)
    {
        ptr+=(18*3*latDivisions*lon);
        ptr+=lat*18*3;
        for(int i = 0; i < 18*3; i++)
        {
            if(i%3==0)
                *ptr = c.r;
            else if(i%3==1)
                *ptr = c.g;
            else if(i%3==2)
                *ptr = c.b;
            ptr++;
        }

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return true;
}

glm::vec3 SphereWorld::colorGradient(GLfloat r)
{
    if(r > 1.0 || r < 0.0)
        return glm::vec3(1, 1, 1);

    int n = r*99;
    switch(n){
        case 0:
            return glm::vec3(0.752941, 0.619608, 0.278431);
            break;
        case 1:
            return glm::vec3(0.74902, 0.619608, 0.278431);
            break;
        case 2:
            return glm::vec3(0.741176, 0.615686, 0.278431);
            break;
        case 3:
            return glm::vec3(0.737255, 0.615686, 0.27451);
            break;
        case 4:
            return glm::vec3(0.729412, 0.615686, 0.27451);
            break;
        case 5:
            return glm::vec3(0.72549, 0.611765, 0.27451);
            break;
        case 6:
            return glm::vec3(0.721569, 0.611765, 0.27451);
            break;
        case 7:
            return glm::vec3(0.713726, 0.611765, 0.270588);
            break;
        case 8:
            return glm::vec3(0.709804, 0.607843, 0.270588);
            break;
        case 9:
            return glm::vec3(0.701961, 0.607843, 0.270588);
            break;
        case 10:
            return glm::vec3(0.698039, 0.603922, 0.270588);
            break;
        case 11:
            return glm::vec3(0.690196, 0.603922, 0.266667);
            break;
        case 12:
            return glm::vec3(0.686275, 0.603922, 0.266667);
            break;
        case 13:
            return glm::vec3(0.682353, 0.6, 0.266667);
            break;
        case 14:
            return glm::vec3(0.67451, 0.6, 0.262745);
            break;
        case 15:
            return glm::vec3(0.670588, 0.6, 0.262745);
            break;
        case 16:
            return glm::vec3(0.662745, 0.596078, 0.262745);
            break;
        case 17:
            return glm::vec3(0.658824, 0.596078, 0.262745);
            break;
        case 18:
            return glm::vec3(0.654902, 0.592157, 0.258824);
            break;
        case 19:
            return glm::vec3(0.647059, 0.592157, 0.258824);
            break;
        case 20:
            return glm::vec3(0.643137, 0.592157, 0.258824);
            break;
        case 21:
            return glm::vec3(0.635294, 0.588235, 0.258824);
            break;
        case 22:
            return glm::vec3(0.631373, 0.588235, 0.254902);
            break;
        case 23:
            return glm::vec3(0.623529, 0.588235, 0.254902);
            break;
        case 24:
            return glm::vec3(0.619608, 0.584314, 0.254902);
            break;
        case 25:
            return glm::vec3(0.615686, 0.584314, 0.25098);
            break;
        case 26:
            return glm::vec3(0.607843, 0.584314, 0.25098);
            break;
        case 27:
            return glm::vec3(0.603922, 0.580392, 0.25098);
            break;
        case 28:
            return glm::vec3(0.596078, 0.580392, 0.25098);
            break;
        case 29:
            return glm::vec3(0.592157, 0.576471, 0.247059);
            break;
        case 30:
            return glm::vec3(0.584314, 0.576471, 0.247059);
            break;
        case 31:
            return glm::vec3(0.580392, 0.576471, 0.247059);
            break;
        case 32:
            return glm::vec3(0.576471, 0.572549, 0.247059);
            break;
        case 33:
            return glm::vec3(0.568627, 0.572549, 0.243137);
            break;
        case 34:
            return glm::vec3(0.564706, 0.572549, 0.243137);
            break;
        case 35:
            return glm::vec3(0.556863, 0.568627, 0.243137);
            break;
        case 36:
            return glm::vec3(0.552941, 0.568627, 0.243137);
            break;
        case 37:
            return glm::vec3(0.54902, 0.564706, 0.239216);
            break;
        case 38:
            return glm::vec3(0.541176, 0.564706, 0.239216);
            break;
        case 39:
            return glm::vec3(0.537255, 0.564706, 0.239216);
            break;
        case 40:
            return glm::vec3(0.529412, 0.560784, 0.235294);
            break;
        case 41:
            return glm::vec3(0.52549, 0.560784, 0.235294);
            break;
        case 42:
            return glm::vec3(0.517647, 0.560784, 0.235294);
            break;
        case 43:
            return glm::vec3(0.513726, 0.556863, 0.235294);
            break;
        case 44:
            return glm::vec3(0.509804, 0.556863, 0.231373);
            break;
        case 45:
            return glm::vec3(0.501961, 0.552941, 0.231373);
            break;
        case 46:
            return glm::vec3(0.498039, 0.552941, 0.231373);
            break;
        case 47:
            return glm::vec3(0.490196, 0.552941, 0.231373);
            break;
        case 48:
            return glm::vec3(0.486275, 0.54902, 0.227451);
            break;
        case 49:
            return glm::vec3(0.482353, 0.54902, 0.227451);
            break;
        case 50:
            return glm::vec3(0.47451, 0.54902, 0.227451);
            break;
        case 51:
            return glm::vec3(0.470588, 0.545098, 0.223529);
            break;
        case 52:
            return glm::vec3(0.462745, 0.545098, 0.223529);
            break;
        case 53:
            return glm::vec3(0.458824, 0.545098, 0.223529);
            break;
        case 54:
            return glm::vec3(0.45098, 0.541176, 0.223529);
            break;
        case 55:
            return glm::vec3(0.447059, 0.541176, 0.219608);
            break;
        case 56:
            return glm::vec3(0.443137, 0.537255, 0.219608);
            break;
        case 57:
            return glm::vec3(0.435294, 0.537255, 0.219608);
            break;
        case 58:
            return glm::vec3(0.431373, 0.537255, 0.219608);
            break;
        case 59:
            return glm::vec3(0.423529, 0.533333, 0.215686);
            break;
        case 60:
            return glm::vec3(0.419608, 0.533333, 0.215686);
            break;
        case 61:
            return glm::vec3(0.411765, 0.533333, 0.215686);
            break;
        case 62:
            return glm::vec3(0.407843, 0.529412, 0.211765);
            break;
        case 63:
            return glm::vec3(0.403922, 0.529412, 0.211765);
            break;
        case 64:
            return glm::vec3(0.396078, 0.52549, 0.211765);
            break;
        case 65:
            return glm::vec3(0.392157, 0.52549, 0.211765);
            break;
        case 66:
            return glm::vec3(0.384314, 0.52549, 0.207843);
            break;
        case 67:
            return glm::vec3(0.380392, 0.521569, 0.207843);
            break;
        case 68:
            return glm::vec3(0.376471, 0.521569, 0.207843);
            break;
        case 69:
            return glm::vec3(0.368627, 0.521569, 0.207843);
            break;
        case 70:
            return glm::vec3(0.364706, 0.517647, 0.203922);
            break;
        case 71:
            return glm::vec3(0.356863, 0.517647, 0.203922);
            break;
        case 72:
            return glm::vec3(0.352941, 0.513726, 0.203922);
            break;
        case 73:
            return glm::vec3(0.345098, 0.513726, 0.203922);
            break;
        case 74:
            return glm::vec3(0.341176, 0.513726, 0.2);
            break;
        case 75:
            return glm::vec3(0.337255, 0.509804, 0.2);
            break;
        case 76:
            return glm::vec3(0.329412, 0.509804, 0.2);
            break;
        case 77:
            return glm::vec3(0.32549, 0.509804, 0.196078);
            break;
        case 78:
            return glm::vec3(0.317647, 0.505882, 0.196078);
            break;
        case 79:
            return glm::vec3(0.313726, 0.505882, 0.196078);
            break;
        case 80:
            return glm::vec3(0.305882, 0.505882, 0.196078);
            break;
        case 81:
            return glm::vec3(0.301961, 0.501961, 0.192157);
            break;
        case 82:
            return glm::vec3(0.298039, 0.501961, 0.192157);
            break;
        case 83:
            return glm::vec3(0.290196, 0.498039, 0.192157);
            break;
        case 84:
            return glm::vec3(0.286275, 0.498039, 0.192157);
            break;
        case 85:
            return glm::vec3(0.278431, 0.498039, 0.188235);
            break;
        case 86:
            return glm::vec3(0.27451, 0.494118, 0.188235);
            break;
        case 87:
            return glm::vec3(0.270588, 0.494118, 0.188235);
            break;
        case 88:
            return glm::vec3(0.262745, 0.494118, 0.184314);
            break;
        case 89:
            return glm::vec3(0.258824, 0.490196, 0.184314);
            break;
        case 90:
            return glm::vec3(0.25098, 0.490196, 0.184314);
            break;
        case 91:
            return glm::vec3(0.247059, 0.486275, 0.184314);
            break;
        case 92:
            return glm::vec3(0.239216, 0.486275, 0.180392);
            break;
        case 93:
            return glm::vec3(0.235294, 0.486275, 0.180392);
            break;
        case 94:
            return glm::vec3(0.231373, 0.482353, 0.180392);
            break;
        case 95:
            return glm::vec3(0.223529, 0.482353, 0.180392);
            break;
        case 96:
            return glm::vec3(0.219608, 0.482353, 0.176471);
            break;
        case 97:
            return glm::vec3(0.211765, 0.478431, 0.176471);
            break;
        case 98:
            return glm::vec3(0.207843, 0.478431, 0.176471);
            break;
        case 99:
            return glm::vec3(0.203922, 0.478431, 0.176471);
            break;
        default:
            return glm::vec3(1, 1, 1);
            break;
    }
}

void SphereWorld::draw()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,18*numTiles);

    if(drawBorder){
        glBindVertexArray(borderVao);
        glDrawArrays(GL_LINES,0,8*numTiles);
    }
}

void SphereWorld::setDrawBorder(GLboolean b){
    drawBorder = b;
}
