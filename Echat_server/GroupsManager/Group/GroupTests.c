#include"Group.h"
#include <stdio.h>


TestCreate1()
{
 Group* group;
 
 if( (group = CreateGroup("NewGroup", "170.0.0.1", 9999)) == NULL)
 {
   printf("TestCreate1 fail\n");
   DestroyGroup(group);
   
   return;
 }
 
 printf("TestCreate1 pass\n");
 DestroyGroup(group);
}


TestCreate2()
{
 Group* group;
 char* ip;
 uint32_t port;
 group = CreateGroup("NewGroup", "170.0.0.1", 9999);
 GroupGetDetails(group, &ip, &port);
 if( port != 9999 || strcmp("170.0.0.1", ip ) || GroupGetSize(group) != 1)
 {
   printf("TestCreate2 fail\n");
   DestroyGroup(group);
   return;
 }
 
 printf("TestCreate2 pass\n");
 DestroyGroup(group);
}

TestIncrease1()
{
 Group* group;
 
 group = CreateGroup("NewGroup", "170.0.0.1", 9999);
 GroupIncreaseSize(group);
 if( GroupGetSize(group) != 2)
 {
   printf("TestIncrease1 fail\n");
   DestroyGroup(group);
   return;
 }
 
 printf("TestIncrease1 pass\n");
 DestroyGroup(group);
}

TestIncrease2()
{
 Group* group;
 
 GroupIncreaseSize(group);
 if( GroupGetSize(group) == 2)
 {
   printf("TestIncrease2 fail\n");
   DestroyGroup(group);
   return;
 }
 
 printf("TestIncrease2 pass\n");
 DestroyGroup(group);
}


TestDecrease1()
{
 Group* group;
 
 group = CreateGroup("NewGroup", "170.0.0.1", 9999);
 GroupDecreaseSize(group);
 if( GroupGetSize(group) != 0)
 {
   printf("TestDecrease1 fail\n");
   DestroyGroup(group);
   return;
 }
 
 printf("TestDecrease1 pass\n");
 DestroyGroup(group);
}


TestGetDetails1()
{
 Group* group;
 char* ip;
 uint32_t port;
 group = CreateGroup("NewGroup", "170.0.0.1", 9999);
 GroupGetDetails(group, &ip, &port);
 if( port != 9999 || strcmp("170.0.0.1", ip ) )
 {
   printf("TestGetDetails1 fail\n");
   DestroyGroup(group);
   return;
 }
 
 printf("TestGetDetails1 pass\n");
 DestroyGroup(group);
}



TestGetSize1()
{
 Group* group;
 
 group = CreateGroup("NewGroup", "170.0.0.1", 9999);

 if( GroupGetSize(group) != 1)
 {
   printf("GetSize1 fail\n");
   DestroyGroup(group);
   return;
 }
 
 printf("GetSize1 pass\n");
 DestroyGroup(group);
}

TestIsEmpty1()
{
  Group* group;
 
 group = CreateGroup("NewGroup", "170.0.0.1", 9999);

 if(IsGroupEmpty(group))
 {
   printf("TestIsEmpty1 fail\n");
   DestroyGroup(group);
   return;
 }
 
 printf("TestIsEmpty1 pass\n");
 DestroyGroup(group);
}

TestIsEmpty2()
{
  Group* group;
 
 group = CreateGroup("NewGroup", "170.0.0.1", 9999);
 GroupDecreaseSize(group);

 if(!IsGroupEmpty(group))
 {
   printf("TestIsEmpty2 fail\n");
   DestroyGroup(group);
   return;
 }
 
 printf("TestIsEmpty2 pass\n");
 DestroyGroup(group);
}

int main()
{
TestCreate1();
TestCreate2();
TestIncrease1();
TestIncrease2(); 
TestDecrease1();
TestGetDetails1();
TestGetSize1();
TestIsEmpty1();
TestIsEmpty2();
}
