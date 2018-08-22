 void readInitialColors(){
   int tempColor, lastColor=-1;
   int nInitialColors=0;
   switchColor = false;  
   while(nInitialColors<3){ 
     if(!switchColor){  
       tempColor = currentColor(); 
       switchColor = (tempColor!=-1) && ((tempColor==0) || (tempColor==1) || (tempColor==2)) && (tempColor!=lastColor);    
     }
     if(switchColor && currentColor()!=tempColor){
       stopColor(tempColor);
       switchColor = false; 
       if(tempColor!=4){//Will stack the colors and the color is not white
         initialColors[nInitialColors] = tempColor;  
       }
//       forwardP_nCM(false, 100);      
       nInitialColors++;        
     }
     if(tempColor != -1)
       lastColor = tempColor;
     forwardP(true);    
   }
 }

 void PRE_LARC_TEST1(){
//   hardCodedCalibration();
//  colorCalibration(0);
//  readInitialColors();  
  while(1){
      backwardP_nWallCM(4.8, false);
      spinP(90);
   }
 }

 // void goUntilBlack(int blackTimes){
//   int tempColor;
//   int blackDetected=0;
//   switchColor = false;  
//   while(blackDetected<blackTimes){ 
//     if(!switchColor){  
//       tempColor = currentColor(); 
//       switchColor = (tempColor!=-1) && (tempColor==3);    
//     }
//     if(switchColor && currentColor()==4){
//       switchColor = false; 
//       blackDetected++;     
//     }
//     forwardPID();
//   }   
//   stop(false);  
// }

// void goUntilBox(bool isRightSharp){ 
//   filtrateDistancesSharp();
//   if(isRightSharp){
//     while(!sharpRight.side){ 
//       forwardPID();
//       filtrateDistancesSharp();
//     }   
//   }
//   else{
//     while(!sharpLeft.side){ 
//       forwardPID();
//       filtrateDistancesSharp();
//     } 
//   }
//   stop(false);  
// }

// void goUntilGreen(){
//   int tempColor;
//   bool greenDetected=false;
//   switchColor = false;  
//   while(!greenDetected){ 
//     if(!switchColor){  
//       tempColor = currentColor(); 
//       switchColor = (tempColor!=-1) && (tempColor==2);    
//     }
//     if(switchColor && currentColor()==4){
//       switchColor = false; 
//       greenDetected=true;     
//     }
//     forwardPID();
//   }   
//   stop(false);     
// }

