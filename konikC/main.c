#include <stdio.h>
#include <stdlib.h>

int szachownica[8][8]={{0}};
int rozmiar = 8;

int ruszSkoczka(int x, int y, int nr)
{
    int rx[8] = {1,2,2,1,-1,-2,-2,-1};
    int ry[8] = {-2,-1,1,2,2,1,-1,-2};
    int tmp_x, tmp_y;
    szachownica[x][y] = nr;
    if(nr == rozmiar * rozmiar)
    {
        for(int i=0 ; i<rozmiar ; i++)
        {
            for(int j=0 ; j<rozmiar ; j++)
                printf("%*d", 4, szachownica[j][i]);
            printf("\n\n");
        }
        return 1;
    }
    else
    {
        for(int i = 0; i < 8; i++)
        {
            tmp_x = x + rx[i];
            tmp_y = y + ry[i];
            if((tmp_x >= 0) && (tmp_x < rozmiar) && (tmp_y >= 0) && (tmp_y < rozmiar) && (szachownica[tmp_x][tmp_y] == 0))
            {
                if(ruszSkoczka(tmp_x, tmp_y, nr+1) == 1)
                {
                    return 1;
                }
            }
        }
        szachownica[x][y] = 0;
    }
    return 0;
}

int main()
{
    int a, b;
    printf("Podaj wspolrzedne miejsca, z ktorego konik ma startowac: \n\n");
    printf("Rzad: ");
    scanf("%d", &a);
    puts("");
    printf("Kolumna: ");
    scanf("%d", &b);
    printf("\n");
    ruszSkoczka(a-1,b-1,1);
    return 0;
}
