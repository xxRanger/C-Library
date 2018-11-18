//
//  main.c
//  C-library
//
//  Created by anxin on 2018/11/18.
//  Copyright © 2018 anxin. All rights reserved.
//

#include "main.h"
#include "HashMap.h"
#include <stdio.h>
#include <sys/time.h>

int main(){
    const int bench = 100000;
    clock_t begin,end;
    HashMap *h = HashMap_New();
    char k[10];
    char addition[10]="2211";
    
    begin = clock();
    for(int i =0;i<bench;++i){
        sprintf(k,"%d",i);
        strcat(k,addition);
        HashMap_Put(h,k,i);
    }
    end = clock();
    printf("%d put use %f\n",bench,(double)(end - begin) / CLOCKS_PER_SEC);
    
    begin = clock();
    for(int i=0;i<bench;++i){
        sprintf(k,"%d",i);
        strcat(k,addition);
        HashMap_GetValue(h,k);
    }
    end = clock();
    printf("%d put use %f\n",bench,(double)(end - begin) / CLOCKS_PER_SEC);
    
    //HashMap_Destroy(h);
}
