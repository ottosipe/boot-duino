function varargout = bootInterface(varargin)
% BOOTINTERFACE MATLAB code for bootInterface.fig
%      BOOTINTERFACE, by itself, creates a new BOOTINTERFACE or raises the existing
%      singleton*.
%
%      H = BOOTINTERFACE returns the handle to a new BOOTINTERFACE or the handle to
%      the existing singleton*.
%
%      BOOTINTERFACE('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in BOOTINTERFACE.M with the given input arguments.
%
%      BOOTINTERFACE('Property','Value',...) creates a new BOOTINTERFACE or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before bootInterface_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to bootInterface_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help bootInterface

% Last Modified by GUIDE v2.5 27-Jun-2012 10:34:03

% Begin initialization code - DO NOT EDIT
gui_Singleton = 0;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @bootInterface_OpeningFcn, ...
                   'gui_OutputFcn',  @bootInterface_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before bootInterface is made visible.
function bootInterface_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to bootInterface (see VARARGIN)

% Choose default command line output for bootInterface
handles.output = hObject;

%close all ports that may be open
delete(instrfindall)

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes bootInterface wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = bootInterface_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

% --- Executes on selection change in serialList.
function serialList_Callback(hObject, eventdata, handles)
% hObject    handle to serialList (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns serialList contents as cell array
%        contents{get(hObject,'Value')} returns selected item from serialList

handles.serialData.currentPort = handles.serialData.portList(get(hObject,'Value'));
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function serialList_CreateFcn(hObject, eventdata, serialData)
% hObject    handle to serialList (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


allPorts = instrhwinfo('serial');
handles.serialData.portList = allPorts.SerialPorts;
handles.serialData.currentPort = handles.serialData.portList(1);
set(hObject,'String',handles.serialData.portList);
guidata(hObject, handles);


function [portHandle] = openThePort(handles,tries) 

    delete(instrfindall)

    portHandle = serial(handles.serialData.currentPort{1});

    portHandle.TimerPeriod=1;
    portHandle.BaudRate=38400;      % define baud rate
    portHandle.Terminator=';';
    
    try
        if(tries > 0)
            fopen(portHandle);
        else
            return;
        end
        disp('Open Port');
    catch err
        tries = tries - 1;
        disp('Retry Port Open');
        portHandle = openThePort(handles,tries);
        
    end

function closeThePort(handles) 

    fclose(handles.port);
    delete(handles.port);
    clear handles.port;

disp('   Port closed');


% --- Executes on button press in connect.
function connect_Callback(hObject, eventdata, handles)
% hObject    handle to connect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if (~isempty(instrfindall))
    set(hObject,'value',0);
    closeThePort(handles);
    set(hObject,'String','Connect');
    guidata(hObject, handles);
else
    handles.port = openThePort(handles,5); %open with 5 tries
    if(isvalid(handles.port))
        guidata(hObject, handles);
        set(hObject,'String','Disconnect');
        plotAway(hObject,handles);
    else 
        disp('Aborted Port Open');
    end
end



function plotAway(hObject,handles)
   
    fprintf(handles.port, '@');

    junk = fscanf(handles.port); 
    firstScan = fscanf(handles.port);
    dataWidth = length(firstScan) - sum(firstScan~=',')  + 1;
    
    plotWindow = 60;%str2double(get(handles.width,'String'));
    %turn off disconnect button
    
    % just a bunch of declerations
    B = zeros(plotWindow,dataWidth);
    C = zeros(0,dataWidth);
    time = [1:plotWindow];

    count = 1;
 
    readString = '%f';
    i = 1;
    while i < dataWidth
        readString = strcat(readString,',%f');
        i = i + 1;
    end
    readString = strcat(readString,';')
    
    tic
    
    
    
    while get(hObject,'value')
        try
        % read line
        
            row = fscanf(handles.port, readString, [dataWidth 1]);

            C(count,:) = row;

            if (mod(count,7)==0)  

                B(end+1,:) = row;
                B(1,:) = [];
                plot(B);
              % axis([0 plotWindow 0 700]);
                drawnow;
            end
            
        catch err
            row
            
        end   
        set(handles.samples,'String',strcat('samples: ', int2str(count)));    
        
        count = count + 1;
    
    
    set(handles.samples,'String',toc);
    
    end
    
    
    assignin('base','output',C);
    


% --- Executes when user attempts to close figure1.
function figure1_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: delete(hObject) closes the figure

%delete(instrfindall);
delete(hObject);


function data_Callback(hObject, eventdata, handles)
% hObject    handle to data (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of data as text
%        str2double(get(hObject,'String')) returns contents of data as a double


% --- Executes during object creation, after setting all properties.
function data_CreateFcn(hObject, eventdata, handles)
% hObject    handle to data (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function width_Callback(hObject, eventdata, handles)
% hObject    handle to width (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of width as text
%        str2double(get(hObject,'String')) returns contents of width as a double


% --- Executes during object creation, after setting all properties.
function width_CreateFcn(hObject, eventdata, handles)
% hObject    handle to width (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
