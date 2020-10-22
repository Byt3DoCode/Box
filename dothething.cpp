#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

int baseAtk, flatAtk, charLv, enemyLv, hitsPerCombo, temp1, temp2=1, temp3, configuration[10]={0}, confStorage[64][10]={0,0}, number_of_crit_in_group[10]={0}, counter, encode[10], decode[10], number_of_group=0;
float hit_number[10], critChance, critDamage, chance_of_group[10], highest_chance=0.0, dmgSum[64]={0.0}, ATK={0.0}, dmgBonus={0.0}, Bonuses, dmg, DefDrop, enemyRes;
bool keepGoing = true;

void readthedamnfile()
{
    fstream fd("e:\\test.txt",ios::in);
    fd >> charLv >> baseAtk >> flatAtk >> ATK >> temp3;
    for ( temp1 = 0; temp1< temp3; ++temp1 )
    {
        fd >> Bonuses;
        dmgBonus += Bonuses;
    }
    fd >> critChance >> critDamage >> hitsPerCombo;
    for ( temp1 = 1; temp1 <= hitsPerCombo; ++temp1 )
        fd >> hit_number [temp1];
    fd >> enemyLv >> DefDrop >> enemyRes;
    fd.close();
};

void prepare()
{
    DefDrop = ( 100 - DefDrop ) / 100;
    ATK /= 100;
    dmgBonus /= 100;
    enemyRes /= 100;
}

float notcrit ( int temp1 )
{
    dmg = ( baseAtk * (1 + ATK ) + flatAtk ) * ( 1 + dmgBonus ) * ( hit_number[temp1] / 100 ) * ( ( 100 + charLv ) / ( ( 100 + charLv ) + ( 100 + enemyLv ) * DefDrop ) ) * ( 1 - enemyRes );
    return dmg;
};

float crit( int temp1 )
{
    dmg = notcrit (temp1) * ( 1 + critDamage / 100);
    return dmg;
};

void next_configuration()
{
    temp1 = hitsPerCombo;
    while( temp1 > 0 && configuration [temp1] == 1 )
    {
        configuration [temp1] = 0;
        --temp1;
    }
    if ( temp1 > 0 )
        configuration [temp1] = 1;
    else
        keepGoing = false;
};

void toStorage ( int confStorage [] [10], int temp2 )
{
    for( temp1 = 1; temp1 <= hitsPerCombo; ++temp1 )
        confStorage [ temp2 ][ temp1 ] = configuration [ temp1 ];
}

void toDamage ( int temp2 )
{
    for ( temp1 = 1; temp1 <= hitsPerCombo; ++temp1 )
    {
        if ( confStorage [ temp2 ][ temp1 ] == 0 )
            dmgSum [ temp2 ] += notcrit ( temp1 );
        else
            dmgSum [ temp2 ] += crit ( temp1 );
    }
}

void critCounter()
{
    counter = 0;
    for ( temp1 = 1 ; temp1 <= hitsPerCombo ; ++temp1 )
        counter += configuration [ temp1 ];
    ++number_of_crit_in_group [ counter ];
    if ( number_of_crit_in_group [ counter ] == 1 )
        ++number_of_group;
    encode [ counter ] = ( encode [ counter ] + temp2 ) * 10;
}

void chance()
{
    for ( temp1 = 0 ; temp1 <= hitsPerCombo ; ++temp1 )
        chance_of_group [ temp1 ] = ( pow ( critChance / 100 , temp1 ) * pow ( ( 100 - critChance ) / 100 , hitsPerCombo - temp1 )  ) * number_of_crit_in_group [ temp1 ] * 100;
    for ( temp1 = 0 ; temp1 <= hitsPerCombo ; ++temp1 )
        if ( chance_of_group [ temp1 ] > highest_chance )
        {
            highest_chance = chance_of_group [ temp1 ];
            temp2 = temp1;
        }
}

void fix()
{
    for ( temp1 = 1 ; temp1 <= number_of_group ; ++temp1 )
        encode [ temp1 ] /= 10;
}

void EXPECTED()
{
    cout << endl << "Expected " << temp2 << " crit(s) with " << highest_chance << " % success rate to deal 1 of the following total damage ";
    temp1 = 0;
    temp3 = encode [ temp2 ];
    while ( temp3 != 0 )
    {
        decode [ temp1 ] = temp3 % 10;
        temp3 /= 10;
        ++temp1;
    }
    for ( temp3 = 0 ; temp3 < temp1 ; ++temp3 )
    {
        cout << dmgSum [ decode [ temp3 ] ] << " ";
    }
    cout << endl;
}
int main()
{
    readthedamnfile();
    prepare();
    while ( keepGoing )
    {
        toStorage ( confStorage , temp2 );
        toDamage ( temp2 );
        critCounter();
        next_configuration();
        ++temp2;
    }
    chance();
    fix();
    EXPECTED();
    return 0;
}
