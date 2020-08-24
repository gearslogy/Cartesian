


:: ------------------------- LIC SERVER----------------------
set "RLM_LICENSE=5053@192.168.8.83"
:: ------------------------- LIC SERVER----------------------






set "KATANA_ENABLE_RECURSIVE_COOKS=1"
set "KATANA_HOME=D:\Program Files\Katana3.6v1"
set "KATANA_TAGLINE=With KtoA 2.1.1.0 and Arnold 5.1.1.0"

set "cartesian_home=D:\plugin_dev\C++\boost_dll\Cartesian_03_VC\KatanaCartesianJIT\bin"
set "CARTESIAN_PLUGINS=D:\plugin_dev\C++\boost_dll\Cartesian_03_VC\KatanaCartesianJIT\bin\cartesian_plugins"
set "path=%cartesian_home%;%path%"
set "KATANA_RESOURCES=d:\Program Files\3Delight\3DelightForKatana;%KATANA_RESOURCES%;%cartesian_home%"

:: ------------------------- START KATANA ---------------------------
"%KATANA_HOME%\bin\katanaBin.exe"
:: ------------------------- START KATANA ---------------------------