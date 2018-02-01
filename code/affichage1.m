
fid = fopen('test.txt', 'r');

A = fgetl(fid);
X=[];
Y=[];



if (strcmp(A,'#Nombreobstacles'))
    A=fgetl(fid);
    kmax=str2num(A);
    for k= 1: kmax
        A=fgetl(fid);
        while(strcmp(A,'#Polygone'))
        A=fgetl(fid);
        smax = str2num(A);

                  A = fgetl(fid);

        for s= 1:smax

                A = str2num(A) ;
                X=[X,A(1)];
                Y=[Y,A(2)];
                A=fgetl(fid);
         end
            fill(X,Y,'r')
            axis([-2 12 -2 12]);
            hold on
            X=[];
            Y=[];
        end
    end
    A=fgetl(fid);
end



%%%% Affichage du D

fid1 = fopen('sol.txt', 'r');
B = fgetl(fid1);
if(strcmp(B,'depart'))
	B=fgetl(fid1);
	B = str2num(B);
	X=[X,B(1)];
    Y=[Y,B(2)];
	scatter(B(1),B(2));
end

%%% Affichage des sommets de la solution sans(A et B)
B = fgetl(fid1);
if (strcmp(B,'solution'))
	B = fgetl(fid1);
	tmax=str2num(B);
    if(tmax==0)
        B = fgetl(fid1);

    else

   for t= 1: tmax
        B=fgetl(fid1);


       B = str2num(B) ;
       X=[X,B(1)];
       Y=[Y,B(2)];


   end
                   B=fgetl(fid1);

    end

           scatter(X,Y);


end

%%


if(strcmp(B,'arrivee'))
	B=fgetl(fid1);
	B = str2num(B);
	X=[X,B(1)];
    Y=[Y,B(2)];
	scatter(B(1),B(2));
end


line(X,Y);


fclose(fid) ;
