
@ECHO OFF

REM "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" 

REM "GROUND SEGMENTATION MODEL (Command Line Options):"

REM "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"

REM "(syntax)"
REM "GroundSegmentationModel.exe --mode [mode] --option1 [option1] --option2 [option2] ...optionN [optionN]"

REM "[MODE: OneFile]"

REM "(options)"
REM "--FilePath [FilePath]"
REM "--OutPutFolder [OutPutFolder]"
REM "--CameraPictureFolder [CameraPictureFolder]"
REM "--CameraCalibrationFilesFolder [path to calib_cam_to_cam.txt and calib_velo_to_cam.txt folder]"

REM "(example)";
REM "GroundSegmentationModel.exe --mode OneFile --FilePath C:\Data\000000.bin --OutPutFolder C:\Output --CameraPictureFolder C:\CameraPictureFolder --CameraCalibrationFilesFolder C:\CamCalibFolder"

REM "[MODE: ManyFiles]"

REM "(options)"
REM "--FolderPath [FolderPath]"
REM "--OutPutFolder [OutPutFolder]"
REM "--CameraPictureFolder [CameraPictureFolder]"
REM "--CameraCalibrationFilesFolder [path to calib_cam_to_cam.txt and calib_velo_to_cam.txt folder]"
REM "--maxNumberOfFiles [max number of files]"

REM "(example)"
REM "GroundSegmentationModel.exe --mode ManyFiles --FolderPath C:\Data --OutPutFolder C:\Output --CameraPictureFolder C:\CameraPictureFolder --CameraCalibrationFilesFolder C:\CamCalibFolder --maxNumberOfFiles 50"

REM "[MODE: SanityCheck]"

REM "(options)"
REM "--FilePath [FilePath]"
REM "--OutPutFolder [OutPutFolder]"
REM "--CameraPictureFolder [CameraPictureFolder]"
REM "--CameraCalibrationFilesFolder [path to calib_cam_to_cam.txt and calib_velo_to_cam.txt folder]"
REM "--doSanityCheckSector [true | false]"
REM "--SanityCheckSector [# of Sector, if equal to 0 runs all Sectors]"
REM "--doSanityCheckBin [true | false]"
REM "--SanityCheckBin [# of Bin, if equal to 0 runs all Bins]"

REM "(example)"
REM "GroundSegmentationModel.exe --mode SanityCheck --FilePath C:\Data\000000.bin --OutPutFolder C:\Output --CameraPictureFolder C:\CameraPictureFolder  --CameraCalibrationFilesFolder C:\CamCalibFolder --doSanityCheckSector true --SanityCheckSector 65 --doSanityCheckBin true --SanityCheckBin 85"

REM "[MODE: ShowDevicesInfo]"

REM "(example)"
REM "GroundSegmentationModel.exe --mode ShowDevicesInfo"

REM "[Additional Options]"

REM "(Parameters)"
REM "--SaveParameters [true | false, Default: false]"
REM  "--SaveDims [true | false, Default: false]";

REM "(Report)"
REM "--Verbose [true | false, Default: false]"
REM "--ShowReport [true | false, Default: false]"
REM "--SaveReport [true | false, Default: false]"

REM "(GPU)"
REM "--Type [gpu | cpu, Default: gpu]"
REM "--Device [#ID of Device, Default: 0]"
REM "--Arithmetic [float | double, Default: float]"
REM "--Platform [#ID of Platform, Default: 0]"

REM "(Results)"
REM "--SaveResult [true | false, Default: false]"
REM "--SavePicture [true | false, Default: true]"
REM "--SaveMovie [true | false, Default: true]"
REM "--FramePerSecond [Frame Per Second, Default: 10]"

REM "(Ground Segmentation Model Parameters)"
REM "--doOnlyFrontView [true | false, Default: true]";
REM "--SetManualMinZ [true | false, Default: false]"
REM "--ManualMinZ [Default: not set]"
REM "--Z_threshold [Default: 8.5]"
REM "--Hg_threshold [Default: 0.10]"
REM "--length_scale [Default: 0.3]"
REM "--sigma_f [Default: 1.3298]"
REM "--sigma_n [Default: 0.1]"
REM "--TData [Default: 80]"
REM "--TModel [Default: 0.5]"
REM "--Tg [Default: 30]"

REM "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"

ECHO GROUND SEGMENTATION MODEL 

SET MAIN_FOLDER="x64\Debug"

IF "%1%"=="OneFile" (

	ECHO [MODE: OneFile]

	SET FilePath="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\Training\000020.bin"
	SET OutPutFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\newtest"
	SET CameraPictureFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_PICTURE\Velodyne\Training"
	SET CameraCalibrationFilesFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\calib"

	%MAIN_FOLDER%\GroundSegmentationModel.exe --mode OneFile --FilePath %FilePath% --OutPutFolder %OutPutFolder% --CameraPictureFolder %CameraPictureFolder% --CameraCalibrationFilesFolder %CameraCalibrationFilesFolder%

)

IF "%1%"=="ManyFiles" (

	ECHO [MODE: ManyFiles]

	SET FolderPath="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\Training"
	SET OutPutFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\cmdTest"
	SET CameraPictureFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_PICTURE\Velodyne\Training"
	SET CameraCalibrationFilesFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\calib"
	SET maxNumberOfFiles=10

	%MAIN_FOLDER%\GroundSegmentationModel.exe --mode ManyFiles --FolderPath %FolderPath% --OutPutFolder %OutPutFolder% --CameraPictureFolder %CameraPictureFolder% --CameraCalibrationFilesFolder %CameraCalibrationFilesFolder% --maxNumberOfFiles %maxNumberOfFiles%

)

IF "%1%"=="CustomOptions" (

	ECHO [MODE: Custom Options - ManyFiles]

	REM SET FolderPath="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\Training"
	SET FilePath="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\Training\000020.bin"
	SET OutPutFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\newtest"
	SET CameraPictureFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_PICTURE\Velodyne\Training"
	SET CameraCalibrationFilesFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\calib"
	SET maxNumberOfFiles=2

	REM "(Parameters)"
	SET SaveParameters=true
	SET SaveDims=true

	REM "(Report)"
	SET Verbose=false
	SET ShowReport=false
	SET SaveReport=true

	REM "(GPU)"
	SET Type=gpu
	SET Device=0
	SET Arithmetic=float
	SET Platform=0

	REM "(Results)"
	SET SaveResult=false
	SET SavePicture=true
	SET SaveMovie=false
	SET FramePerSecond=10

	REM "(Ground Segmentation Model Parameters)"
	SET SetManualMinZ=true
	SET ManualMinZ=-1.0
	SET Z_threshold=8.5
	SET Hg_threshold=0.10
	SET length_scale=0.3
	SET sigma_f=1.3298
	SET sigma_n=0.1
	SET TData=80
	SET TModel=0.5
	SET Tg=30

	%MAIN_FOLDER%\GroundSegmentationModel.exe --mode OneFile --FilePath %FilePath% --OutPutFolder %OutPutFolder% --CameraPictureFolder %CameraPictureFolder% --CameraCalibrationFilesFolder %CameraCalibrationFilesFolder% --maxNumberOfFiles %maxNumberOfFiles%^

	--SaveParameters %SaveParameters% --SaveDims %SaveDims% --Verbose %Verbose% --ShowReport %ShowReport% --SaveReport %SaveReport% --Type %Type% --Device %Device% --Arithmetic %Arithmetic% --Platform %Platform% --SaveResult %SaveResult%^

	--SavePicture %SavePicture% --SaveMovie %SaveMovie% --FramePerSecond %FramePerSecond% --SetManualMinZ %SetManualMinZ% --ManualMinZ %ManualMinZ% --Z_threshold %Z_threshold% --Hg_threshold %Hg_threshold% --length_scale %length_scale%^

	--sigma_n %sigma_n% --sigma_f %sigma_f% --TData %TData% --TModel %TModel% --Tg %Tg% 

)


IF "%1%"=="SanityCheck" (

	ECHO [MODE: SanityCheck]

	SET FilePath="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\Training\000020.bin"
	SET OutPutFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\newtest"
	SET CameraPictureFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_PICTURE\Velodyne\Training"
	SET CameraCalibrationFilesFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\calib"
	SET doSanityCheckSector=true
	SET SanityCheckSector=0
	SET doSanityCheckBin=true
	SET SanityCheckBin=0

	%MAIN_FOLDER%\GroundSegmentationModel.exe --mode SanityCheck --FilePath %FilePath% --OutPutFolder %OutPutFolder% --CameraPictureFolder %CameraPictureFolder%  --CameraCalibrationFilesFolder %CameraCalibrationFilesFolder% --doSanityCheckSector %doSanityCheckSector% --SanityCheckSector %SanityCheckSector% --doSanityCheckBin %doSanityCheckBin% --SanityCheckBin %SanityCheckBin%

)

IF "%1%"=="ShowDeviceInfo" (
	
	ECHO [MODE: ShowDevicesInfo]

	%MAIN_FOLDER%\GroundSegmentationModel.exe --mode ShowDeviceInfo

)

IF "%1%"=="SeveralModels" (

	ECHO [MODE: SeveralModels]

	REM "(Parameters)"
	SET SaveParameters=true
	SET SaveDims=false

	REM "(Report)"
	SET Verbose=false
	SET ShowReport=false
	SET SaveReport=true

	REM "(GPU)"
	SET Type=gpu
	SET Device=0
	SET Arithmetic=float
	SET Platform=0

	REM "(Results)"
	SET SaveResult=false
	SET SavePicture=true
	SET SaveMovie=true
	SET FramePerSecond=10

	REM "(Ground Segmentation Model Parameters)"
	SET SetManualMinZ=true
	SET ManualMinZ=-2.0
	SET Z_threshold=8.5
	SET Hg_threshold=0.10
	SET length_scale=0.3
	SET sigma_n=1.3298
	SET sigma_f=0.1
	SET TData=0.8
	SET TModel=0.005
	SET Tg=0.3

	if "%2%"=="01" (

	ECHO [DATA: 2011_09_26_drive_0001_sync]

	SET FolderPath="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0001_sync\2011_09_26\2011_09_26_drive_0001_sync\velodyne_points\data"
	SET OutPutFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_OUT\2011_09_26_drive_0001_sync"
	SET CameraPictureFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0001_sync\2011_09_26\2011_09_26_drive_0001_sync\image_00\data"
	SET CameraCalibrationFilesFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\calib"
	SET maxNumberOfFiles=108

	%MAIN_FOLDER%\GroundSegmentationModel.exe --mode ManyFiles --FolderPath %FolderPath% --OutPutFolder %OutPutFolder% --CameraPictureFolder %CameraPictureFolder% --CameraCalibrationFilesFolder %CameraCalibrationFilesFolder% --maxNumberOfFiles %maxNumberOfFiles%^

	--SaveParameters %SaveParameters% --Verbose %Verbose% --ShowReport %ShowReport% --SaveReport %SaveReport% --Type %Type% --Device %Device% --Arithmetic %Arithmetic% --Platform %Platform% --SaveResult %SaveResult%^

	--SavePicture %SavePicture% --SaveMovie %SaveMovie% --FramePerSecond %FramePerSecond% --SetManualMinZ %SetManualMinZ% --ManualMinZ %ManualMinZ% --Z_threshold %Z_threshold% --Hg_threshold %Hg_threshold% --length_scale %length_scale%^

	--sigma_n %sigma_n% --sigma_f %sigma_f% --TData %TData% --TModel %TModel% --Tg %Tg% 

	)

	if "%2%"=="02" (

	ECHO [DATA: 2011_09_26_drive_0002_sync]

	SET FolderPath="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0002_sync\2011_09_26\2011_09_26_drive_0002_sync\velodyne_points\data"
	SET OutPutFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_OUT\2011_09_26_drive_0002_sync"
	SET CameraPictureFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0002_sync\2011_09_26\2011_09_26_drive_0002_sync\image_00\data"
	SET CameraCalibrationFilesFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\calib"
	SET maxNumberOfFiles=77

	%MAIN_FOLDER%\GroundSegmentationModel.exe --mode ManyFiles --FolderPath %FolderPath% --OutPutFolder %OutPutFolder% --CameraPictureFolder %CameraPictureFolder% --CameraCalibrationFilesFolder %CameraCalibrationFilesFolder% --maxNumberOfFiles %maxNumberOfFiles%^

	--SaveParameters %SaveParameters% --Verbose %Verbose% --ShowReport %ShowReport% --SaveReport %SaveReport% --Type %Type% --Device %Device% --Arithmetic %Arithmetic% --Platform %Platform% --SaveResult %SaveResult%^

	--SavePicture %SavePicture% --SaveMovie %SaveMovie% --FramePerSecond %FramePerSecond% --SetManualMinZ %SetManualMinZ% --ManualMinZ %ManualMinZ% --Z_threshold %Z_threshold% --Hg_threshold %Hg_threshold% --length_scale %length_scale%^

	--sigma_n %sigma_n% --sigma_f %sigma_f% --TData %TData% --TModel %TModel% --Tg %Tg% 

	)

	if "%2%"=="05" (

	ECHO [DATA: 2011_09_26_drive_0005_sync]

	SET FolderPath="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0005_sync\2011_09_26\2011_09_26_drive_0005_sync\velodyne_points\data"
	SET OutPutFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_OUT\2011_09_26_drive_0005_sync"
	SET CameraPictureFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0005_sync\2011_09_26\2011_09_26_drive_0005_sync\image_00\data"
	SET CameraCalibrationFilesFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\calib"
	SET maxNumberOfFiles=154

	%MAIN_FOLDER%\GroundSegmentationModel.exe --mode ManyFiles --FolderPath %FolderPath% --OutPutFolder %OutPutFolder% --CameraPictureFolder %CameraPictureFolder% --CameraCalibrationFilesFolder %CameraCalibrationFilesFolder% --maxNumberOfFiles %maxNumberOfFiles%^

	--SaveParameters %SaveParameters% --Verbose %Verbose% --ShowReport %ShowReport% --SaveReport %SaveReport% --Type %Type% --Device %Device% --Arithmetic %Arithmetic% --Platform %Platform% --SaveResult %SaveResult%^

	--SavePicture %SavePicture% --SaveMovie %SaveMovie% --FramePerSecond %FramePerSecond% --SetManualMinZ %SetManualMinZ% --ManualMinZ %ManualMinZ% --Z_threshold %Z_threshold% --Hg_threshold %Hg_threshold% --length_scale %length_scale%^

	--sigma_n %sigma_n% --sigma_f %sigma_f% --TData %TData% --TModel %TModel% --Tg %Tg% 

	)

	if "%2%"=="09" (

	ECHO [DATA: 2011_09_26_drive_0009_sync]

	SET FolderPath="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0009_sync\2011_09_26\2011_09_26_drive_0009_sync\velodyne_points\data"
	SET OutPutFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_OUT\2011_09_26_drive_0009_sync"
	SET CameraPictureFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0009_sync\2011_09_26\2011_09_26_drive_0009_sync\image_00\data"
	SET CameraCalibrationFilesFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\calib"
	SET maxNumberOfFiles=443

	%MAIN_FOLDER%\GroundSegmentationModel.exe --mode ManyFiles --FolderPath %FolderPath% --OutPutFolder %OutPutFolder% --CameraPictureFolder %CameraPictureFolder% --CameraCalibrationFilesFolder %CameraCalibrationFilesFolder% --maxNumberOfFiles %maxNumberOfFiles%^

	--SaveParameters %SaveParameters% --Verbose %Verbose% --ShowReport %ShowReport% --SaveReport %SaveReport% --Type %Type% --Device %Device% --Arithmetic %Arithmetic% --Platform %Platform% --SaveResult %SaveResult%^

	--SavePicture %SavePicture% --SaveMovie %SaveMovie% --FramePerSecond %FramePerSecond% --SetManualMinZ %SetManualMinZ% --ManualMinZ %ManualMinZ% --Z_threshold %Z_threshold% --Hg_threshold %Hg_threshold% --length_scale %length_scale%^

	--sigma_n %sigma_n% --sigma_f %sigma_f% --TData %TData% --TModel %TModel% --Tg %Tg% 

	)

	if "%2%"=="11" (

	ECHO [DATA: 2011_09_26_drive_0011_sync]

	SET FolderPath="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0011_sync\2011_09_26\2011_09_26_drive_0011_sync\velodyne_points\data"
	SET OutPutFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_OUT\2011_09_26_drive_0011_sync"
	SET CameraPictureFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0011_sync\2011_09_26\2011_09_26_drive_0011_sync\image_00\data"
	SET CameraCalibrationFilesFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\calib"
	SET maxNumberOfFiles=233

	%MAIN_FOLDER%\GroundSegmentationModel.exe --mode ManyFiles --FolderPath %FolderPath% --OutPutFolder %OutPutFolder% --CameraPictureFolder %CameraPictureFolder% --CameraCalibrationFilesFolder %CameraCalibrationFilesFolder% --maxNumberOfFiles %maxNumberOfFiles%^

	--SaveParameters %SaveParameters% --Verbose %Verbose% --ShowReport %ShowReport% --SaveReport %SaveReport% --Type %Type% --Device %Device% --Arithmetic %Arithmetic% --Platform %Platform% --SaveResult %SaveResult%^

	--SavePicture %SavePicture% --SaveMovie %SaveMovie% --FramePerSecond %FramePerSecond% --SetManualMinZ %SetManualMinZ% --ManualMinZ %ManualMinZ% --Z_threshold %Z_threshold% --Hg_threshold %Hg_threshold% --length_scale %length_scale%^

	--sigma_n %sigma_n% --sigma_f %sigma_f% --TData %TData% --TModel %TModel% --Tg %Tg% 

	)

	if "%2%"=="13" (

	ECHO [DATA: 2011_09_26_drive_0013_sync]

	SET FolderPath="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0013_sync\2011_09_26\2011_09_26_drive_0013_sync\velodyne_points\data"
	SET OutPutFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_OUT\2011_09_26_drive_0013_sync"
	SET CameraPictureFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0013_sync\2011_09_26\2011_09_26_drive_0013_sync\image_00\data"
	SET CameraCalibrationFilesFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\calib"
	SET maxNumberOfFiles=144

	%MAIN_FOLDER%\GroundSegmentationModel.exe --mode ManyFiles --FolderPath %FolderPath% --OutPutFolder %OutPutFolder% --CameraPictureFolder %CameraPictureFolder% --CameraCalibrationFilesFolder %CameraCalibrationFilesFolder% --maxNumberOfFiles %maxNumberOfFiles%^

	--SaveParameters %SaveParameters% --Verbose %Verbose% --ShowReport %ShowReport% --SaveReport %SaveReport% --Type %Type% --Device %Device% --Arithmetic %Arithmetic% --Platform %Platform% --SaveResult %SaveResult%^

	--SavePicture %SavePicture% --SaveMovie %SaveMovie% --FramePerSecond %FramePerSecond% --SetManualMinZ %SetManualMinZ% --ManualMinZ %ManualMinZ% --Z_threshold %Z_threshold% --Hg_threshold %Hg_threshold% --length_scale %length_scale%^

	--sigma_n %sigma_n% --sigma_f %sigma_f% --TData %TData% --TModel %TModel% --Tg %Tg% 

	)

	if "%2%"=="14" (

	ECHO [DATA: 2011_09_26_drive_0014_sync]

	SET FolderPath="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0014_sync\2011_09_26\2011_09_26_drive_0014_sync\velodyne_points\data"
	SET OutPutFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_OUT\2011_09_26_drive_0014_sync"
	SET CameraPictureFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_26_drive_0014_sync\2011_09_26\2011_09_26_drive_0014_sync\image_00\data"
	SET CameraCalibrationFilesFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\calib"
	SET maxNumberOfFiles=314

	%MAIN_FOLDER%\GroundSegmentationModel.exe --mode ManyFiles --FolderPath %FolderPath% --OutPutFolder %OutPutFolder% --CameraPictureFolder %CameraPictureFolder% --CameraCalibrationFilesFolder %CameraCalibrationFilesFolder% --maxNumberOfFiles %maxNumberOfFiles%^

	--SaveParameters %SaveParameters% --Verbose %Verbose% --ShowReport %ShowReport% --SaveReport %SaveReport% --Type %Type% --Device %Device% --Arithmetic %Arithmetic% --Platform %Platform% --SaveResult %SaveResult%^

	--SavePicture %SavePicture% --SaveMovie %SaveMovie% --FramePerSecond %FramePerSecond% --SetManualMinZ %SetManualMinZ% --ManualMinZ %ManualMinZ% --Z_threshold %Z_threshold% --Hg_threshold %Hg_threshold% --length_scale %length_scale%^

	--sigma_n %sigma_n% --sigma_f %sigma_f% --TData %TData% --TModel %TModel% --Tg %Tg% 

	)

	if "%2%"=="71" (

	ECHO [DATA: 2011_09_29_drive_0071_sync]

	SET FolderPath="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_29_drive_0071_sync\2011_09_29\2011_09_29_drive_0071_sync\velodyne_points\data"
	SET OutPutFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_OUT\2011_09_29_drive_0071_sync"
	SET CameraPictureFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\2011_09_29_drive_0071_sync\2011_09_29\2011_09_29_drive_0071_sync\image_00\data"
	SET CameraCalibrationFilesFolder="C:\Users\Dornas\Hard Quale .com\Kod-Demand - Documents\_CLIENTS\SEOULROBOTICS\_DATA\Velodyne\calib"
	SET maxNumberOfFiles=1059

	%MAIN_FOLDER%\GroundSegmentationModel.exe --mode ManyFiles --FolderPath %FolderPath% --OutPutFolder %OutPutFolder% --CameraPictureFolder %CameraPictureFolder% --CameraCalibrationFilesFolder %CameraCalibrationFilesFolder% --maxNumberOfFiles %maxNumberOfFiles%^

	--SaveParameters %SaveParameters% --Verbose %Verbose% --ShowReport %ShowReport% --SaveReport %SaveReport% --Type %Type% --Device %Device% --Arithmetic %Arithmetic% --Platform %Platform% --SaveResult %SaveResult%^

	--SavePicture %SavePicture% --SaveMovie %SaveMovie% --FramePerSecond %FramePerSecond% --SetManualMinZ %SetManualMinZ% --ManualMinZ %ManualMinZ% --Z_threshold %Z_threshold% --Hg_threshold %Hg_threshold% --length_scale %length_scale%^

	--sigma_n %sigma_n% --sigma_f %sigma_f% --TData %TData% --TModel %TModel% --Tg %Tg% 

	)

)

