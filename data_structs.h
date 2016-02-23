#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include<QDateTime>

#define LOG_PACKET_TYPE_E	1
#define LOG_PACKET_TYPE_S	2
#define LOG_PACKET_TYPE_W	3

struct packetEnviron{
    int temp_outer;
    int temp_inner;
    int hum_inner;
    unsigned int flags;
};

struct packetPotState
{
    int doser;
    int bowl;
    int value;
    unsigned int flags;
};


struct packetPotWatering
{
    int doser;
    int bowl;
    int portion;
};

struct packetWrapper{
    QDateTime time;
    int type;
    union{
        packetEnviron pe;
        packetPotState ps;
        packetPotWatering pw;
    };
};

class PlantData{
public:
    unsigned int flags;
    int index;
    int chip;
    int pin;
    int X;
    int Y;
    int min;
    int max;
    int daymax;
    int portion;
    int pgm;
    int en;
    QString name;
/*public:
    bool operator<(const PlantData b) const
    {
        return this->bowl - b.bowl < 0;
    }*/
};


#endif // DATA_STRUCTS_H
