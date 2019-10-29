#include <vector>
#include <citro2d.h>
#include <3ds.h>

enum RemoteTouchID{
    REMOTEREV = 0,
    REMOTESTOP = 1,
    REMOTEPLAY = 2,
    REMOTEPAUSE = 3,
    REMOTEFF = 4,
    REMOTEDOWN = 5,
    REMOTELEFT = 6,
    REMOTEUP = 7,
    REMOTERIGHT = 8,
    REMOTEOK = 9,
    REMOTEBACK = 10,
    REMOTEINFO = 11,
    REMOTEHOME = 12,
    REMOTECONTEXT = 13

};

typedef struct
{
	C2D_Sprite spr;
	float px, py;
} Sprite;


class CGFX{
public:
    void Init();
    int RemoteMenutouchSprite(touchPosition *touch);

    C2D_SpriteSheet RemoteMenuspriteSheet;
    //Sprite sprites[9];

    std::vector<Sprite> remotemenuspritevector;




};
