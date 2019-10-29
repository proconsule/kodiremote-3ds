
#include "GFX.hpp"


void CGFX::Init(){

    RemoteMenuspriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/remotesprites.t3x");

    for (size_t i = 0; i < 5; i++)
	{
	    Sprite sprite;
	    C2D_SpriteFromSheet(&sprite.spr, RemoteMenuspriteSheet, i);
	    C2D_SpriteSetCenter(&sprite.spr, 0.5f, 0.5f);
	    C2D_SpriteSetRotation(&sprite.spr, C3D_Angle(0.0f));
	    float x = 20.0f + (36.0f +33.0f )*i;
	    float y = 240.f-40.0f;
	    C2D_SpriteSetPos(&sprite.spr, x , y);
	    remotemenuspritevector.push_back(sprite);

	}
    Sprite arrow1;
    C2D_SpriteFromSheet(&arrow1.spr, RemoteMenuspriteSheet, 5);
    C2D_SpriteSetCenter(&arrow1.spr, 0.5f, 0.5f);
    C2D_SpriteRotateDegrees(&arrow1.spr, 0.0f);
    float x = 20.0f + (36.0f )*1;
    float y = 240.f-50.f-36.0f-36.0f;
    C2D_SpriteSetPos(&arrow1.spr, x , y);

    Sprite arrow2;
    C2D_SpriteFromSheet(&arrow2.spr, RemoteMenuspriteSheet, 5);
    C2D_SpriteSetCenter(&arrow2.spr, 0.5f, 0.5f);
    C2D_SpriteRotateDegrees(&arrow2.spr, 90.0f);
    x = 20.0f + (36.0f )*0;
    y = 240.f-50.f-36.0f-36.0f-36.0f;
    C2D_SpriteSetPos(&arrow2.spr, x , y);

    Sprite arrow3;
    C2D_SpriteFromSheet(&arrow3.spr, RemoteMenuspriteSheet, 5);
    C2D_SpriteSetCenter(&arrow3.spr, 0.5f, 0.5f);
    C2D_SpriteRotateDegrees(&arrow3.spr, 180.0f);
    x = 20.0f + (36.0f )*1;
    y = 240.f-50.f-36.0f-36.0f-36.0f-36.0f;
    C2D_SpriteSetPos(&arrow3.spr, x , y);

    Sprite arrow4;
    C2D_SpriteFromSheet(&arrow4.spr, RemoteMenuspriteSheet, 5);
    C2D_SpriteSetCenter(&arrow4.spr, 0.5f, 0.5f);
    C2D_SpriteRotateDegrees(&arrow4.spr, 270.0f);
    x = 20.0f + (36.0f )*2;
    y = 240.f-50.f-36.0f-36.0f-36.0f;
    C2D_SpriteSetPos(&arrow4.spr, x , y);

    remotemenuspritevector.push_back(arrow1);
    remotemenuspritevector.push_back(arrow2);
    remotemenuspritevector.push_back(arrow3);
    remotemenuspritevector.push_back(arrow4);

    Sprite okbut;
    C2D_SpriteFromSheet(&okbut.spr, RemoteMenuspriteSheet, 6);
    C2D_SpriteSetCenter(&okbut.spr, 0.5f, 0.5f);
    C2D_SpriteRotateDegrees(&okbut.spr, 0.0f);
    x = 320.0f - okbut.spr.params.pos.w/2 -10.0f;
    y = (okbut.spr.params.pos.h/2) +10.0f;
    C2D_SpriteSetPos(&okbut.spr, x , y);
    remotemenuspritevector.push_back(okbut);

    Sprite backbut;
    C2D_SpriteFromSheet(&backbut.spr, RemoteMenuspriteSheet, 7);
    C2D_SpriteSetCenter(&backbut.spr, 0.5f, 0.5f);
    C2D_SpriteRotateDegrees(&backbut.spr, 0.0f);
    x = 320.0f - backbut.spr.params.pos.w/2 -10.0f;
    y = (okbut.spr.params.pos.h/2)+ okbut.spr.params.pos.h +(10.0f*2);
    C2D_SpriteSetPos(&backbut.spr, x , y);
    remotemenuspritevector.push_back(backbut);

    Sprite infobut;
    C2D_SpriteFromSheet(&infobut.spr, RemoteMenuspriteSheet, 8);
    C2D_SpriteSetCenter(&infobut.spr, 0.5f, 0.5f);
    C2D_SpriteRotateDegrees(&infobut.spr, 0.0f);
    x = 320.0f - infobut.spr.params.pos.w/2 -10.0f;
    y = (okbut.spr.params.pos.h/2)+ (okbut.spr.params.pos.h*2) +(10.0f*3);
    C2D_SpriteSetPos(&infobut.spr, x , y);
    remotemenuspritevector.push_back(infobut);

    Sprite homebut;
    C2D_SpriteFromSheet(&homebut.spr, RemoteMenuspriteSheet, 9);
    C2D_SpriteSetCenter(&homebut.spr, 0.5f, 0.5f);
    C2D_SpriteRotateDegrees(&homebut.spr, 0.0f);
    x = 320.0f - homebut.spr.params.pos.w/2 -10.0f - homebut.spr.params.pos.w -10.0f;
    y = (okbut.spr.params.pos.h/2) +10.0f;
    C2D_SpriteSetPos(&homebut.spr, x , y);
    remotemenuspritevector.push_back(homebut);

    Sprite contextbut;
    C2D_SpriteFromSheet(&contextbut.spr, RemoteMenuspriteSheet, 10);
    C2D_SpriteSetCenter(&contextbut.spr, 0.5f, 0.5f);
    C2D_SpriteRotateDegrees(&contextbut.spr, 0.0f);
    x = 320.0f - contextbut.spr.params.pos.w/2 -10.0f - contextbut.spr.params.pos.w -10.0f;
    y = (okbut.spr.params.pos.h/2) + okbut.spr.params.pos.h +(10.0f*2);
    C2D_SpriteSetPos(&contextbut.spr, x , y);
    remotemenuspritevector.push_back(contextbut);


}

int CGFX::RemoteMenutouchSprite(touchPosition *touch){

        for (size_t i = 0; i < remotemenuspritevector.size(); i++)
	{
        Sprite* sprite = &remotemenuspritevector[i];
	    if(touch->px > (sprite->spr.params.pos.x - (sprite->spr.params.pos.w/2.0)) && touch->px < (sprite->spr.params.pos.x + (sprite->spr.params.pos.w/2.0))){
           if(touch->py > (sprite->spr.params.pos.y - (sprite->spr.params.pos.h/2.0)) && touch->py < (sprite->spr.params.pos.y + (sprite->spr.params.pos.h/2.0))){
                return i;
           }
        }
	}


     return -1;
}
