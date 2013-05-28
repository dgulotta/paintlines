<!DOCTYPE UI><UI version="3.3" stdsetdef="1">
<class>QuasiStripesForm</class>
<widget class="QMainWindow">
    <property name="name">
        <cstring>QuasiStripesForm</cstring>
    </property>
    <property name="geometry">
        <rect>
            <x>0</x>
            <y>0</y>
            <width>504</width>
            <height>434</height>
        </rect>
    </property>
    <property name="caption">
        <string>Paintstripes</string>
    </property>
    <widget class="QLabel">
        <property name="name">
            <cstring>textLabel1</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>10</x>
                <y>60</y>
                <width>75</width>
                <height>21</height>
            </rect>
        </property>
        <property name="text">
            <string>Size</string>
        </property>
    </widget>
    <widget class="QComboBox">
        <item>
            <property name="text">
                <string>CM</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>CMM</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>P1</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>P2</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>P3</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>P31M</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>P3M1</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>P4</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>P4G</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>P4M</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>P6</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>P6M</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>PG</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>PGG</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>PM</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>PMG</string>
            </property>
        </item>
        <item>
            <property name="text">
                <string>PMM</string>
            </property>
        </item>
        <property name="name">
            <cstring>ComboSymmetry</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>10</x>
                <y>30</y>
                <width>100</width>
                <height>26</height>
            </rect>
        </property>
        <property name="sizeLimit">
            <number>20</number>
        </property>
    </widget>
    <widget class="QLabel">
        <property name="name">
            <cstring>textLabel1_2</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>10</x>
                <y>10</y>
                <width>100</width>
                <height>20</height>
            </rect>
        </property>
        <property name="text">
            <string>Symmetry</string>
        </property>
    </widget>
    <widget class="QLineEdit">
        <property name="name">
            <cstring>EditSize</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>10</x>
                <y>80</y>
                <width>100</width>
                <height>25</height>
            </rect>
        </property>
        <property name="text">
            <string>256</string>
        </property>
    </widget>
    <widget class="quasiperiodic_paintstripeswidget">
        <property name="name">
            <cstring>StripesFrame</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>150</x>
                <y>10</y>
                <width>360</width>
                <height>360</height>
            </rect>
        </property>
    </widget>
    <widget class="QLabel">
        <property name="name">
            <cstring>textLabel2_3_2</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>70</x>
                <y>190</y>
                <width>16</width>
                <height>20</height>
            </rect>
        </property>
        <property name="text">
            <string>Y</string>
        </property>
    </widget>
    <widget class="QSpinBox">
        <property name="name">
            <cstring>SpinXTile</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>30</x>
                <y>190</y>
                <width>40</width>
                <height>26</height>
            </rect>
        </property>
        <property name="minValue">
            <number>1</number>
        </property>
        <property name="value">
            <number>2</number>
        </property>
    </widget>
    <widget class="QSpinBox">
        <property name="name">
            <cstring>SpinYTile</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>90</x>
                <y>190</y>
                <width>40</width>
                <height>26</height>
            </rect>
        </property>
        <property name="minValue">
            <number>1</number>
        </property>
        <property name="value">
            <number>2</number>
        </property>
    </widget>
    <widget class="QPushButton">
        <property name="name">
            <cstring>ButtonRestore</cstring>
        </property>
        <property name="enabled">
            <bool>false</bool>
        </property>
        <property name="geometry">
            <rect>
                <x>10</x>
                <y>250</y>
                <width>130</width>
                <height>31</height>
            </rect>
        </property>
        <property name="text">
            <string>Restore Original</string>
        </property>
    </widget>
    <widget class="QLabel">
        <property name="name">
            <cstring>textLabel2_3</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>10</x>
                <y>190</y>
                <width>16</width>
                <height>20</height>
            </rect>
        </property>
        <property name="text">
            <string>X</string>
        </property>
    </widget>
    <widget class="QLabel">
        <property name="name">
            <cstring>textLabel1_3</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>10</x>
                <y>170</y>
                <width>50</width>
                <height>20</height>
            </rect>
        </property>
        <property name="text">
            <string>Tiles</string>
        </property>
    </widget>
    <widget class="QPushButton">
        <property name="name">
            <cstring>ButtonRandomize</cstring>
        </property>
        <property name="enabled">
            <bool>false</bool>
        </property>
        <property name="geometry">
            <rect>
                <x>10</x>
                <y>220</y>
                <width>130</width>
                <height>31</height>
            </rect>
        </property>
        <property name="text">
            <string>Randomize</string>
        </property>
    </widget>
    <widget class="QLabel">
        <property name="name">
            <cstring>textLabel1_4</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>10</x>
                <y>110</y>
                <width>76</width>
                <height>21</height>
            </rect>
        </property>
        <property name="text">
            <string>Alpha</string>
        </property>
    </widget>
    <widget class="QLineEdit">
        <property name="name">
            <cstring>EditAlpha</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>60</x>
                <y>110</y>
                <width>81</width>
                <height>26</height>
            </rect>
        </property>
        <property name="text">
            <string>1.0</string>
        </property>
    </widget>
    <widget class="QPushButton">
        <property name="name">
            <cstring>ButtonDraw</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>10</x>
                <y>140</y>
                <width>102</width>
                <height>28</height>
            </rect>
        </property>
        <property name="text">
            <string>Draw</string>
        </property>
    </widget>
</widget>
<menubar>
    <property name="name">
        <cstring>MenuBar</cstring>
    </property>
    <item text="&amp;File" name="fileMenu">
        <action name="fileNewAction"/>
        <action name="fileOpenAction"/>
        <action name="fileSaveAction"/>
        <action name="fileSaveAsAction"/>
        <separator/>
        <action name="filePrintAction"/>
        <separator/>
        <action name="fileExitAction"/>
    </item>
    <item text="&amp;Edit" name="editMenu">
        <action name="editUndoAction"/>
        <action name="editRedoAction"/>
        <separator/>
        <action name="editCutAction"/>
        <action name="editCopyAction"/>
        <action name="editPasteAction"/>
        <separator/>
        <action name="editFindAction"/>
    </item>
    <item text="&amp;Help" name="helpMenu">
        <action name="helpContentsAction"/>
        <action name="helpIndexAction"/>
        <separator/>
        <action name="helpAboutAction"/>
    </item>
</menubar>
<toolbars>
</toolbars>
<customwidgets>
    <customwidget>
        <class>quasiperiodic_paintstripeswidget</class>
        <header location="local">quasiperiodic_paintstripeswidget.h</header>
        <sizehint>
            <width>-1</width>
            <height>-1</height>
        </sizehint>
        <container>0</container>
        <sizepolicy>
            <hordata>5</hordata>
            <verdata>5</verdata>
            <horstretch>0</horstretch>
            <verstretch>0</verstretch>
        </sizepolicy>
        <pixmap>image0</pixmap>
    </customwidget>
</customwidgets>
<actions>
    <action>
        <property name="name">
            <cstring>fileNewAction</cstring>
        </property>
        <property name="iconSet">
            <iconset></iconset>
        </property>
        <property name="text">
            <string>New</string>
        </property>
        <property name="menuText">
            <string>&amp;New</string>
        </property>
        <property name="accel">
            <string>Ctrl+N</string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>fileOpenAction</cstring>
        </property>
        <property name="iconSet">
            <iconset></iconset>
        </property>
        <property name="text">
            <string>Open</string>
        </property>
        <property name="menuText">
            <string>&amp;Open...</string>
        </property>
        <property name="accel">
            <string>Ctrl+O</string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>fileSaveAction</cstring>
        </property>
        <property name="iconSet">
            <iconset></iconset>
        </property>
        <property name="text">
            <string>Save</string>
        </property>
        <property name="menuText">
            <string>&amp;Save</string>
        </property>
        <property name="accel">
            <string>Ctrl+S</string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>fileSaveAsAction</cstring>
        </property>
        <property name="text">
            <string>Save As</string>
        </property>
        <property name="menuText">
            <string>Save &amp;As...</string>
        </property>
        <property name="accel">
            <string></string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>filePrintAction</cstring>
        </property>
        <property name="iconSet">
            <iconset></iconset>
        </property>
        <property name="text">
            <string>Print</string>
        </property>
        <property name="menuText">
            <string>&amp;Print...</string>
        </property>
        <property name="accel">
            <string>Ctrl+P</string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>fileExitAction</cstring>
        </property>
        <property name="text">
            <string>Exit</string>
        </property>
        <property name="menuText">
            <string>E&amp;xit</string>
        </property>
        <property name="accel">
            <string></string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>editUndoAction</cstring>
        </property>
        <property name="iconSet">
            <iconset></iconset>
        </property>
        <property name="text">
            <string>Undo</string>
        </property>
        <property name="menuText">
            <string>&amp;Undo</string>
        </property>
        <property name="accel">
            <string>Ctrl+Z</string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>editRedoAction</cstring>
        </property>
        <property name="iconSet">
            <iconset></iconset>
        </property>
        <property name="text">
            <string>Redo</string>
        </property>
        <property name="menuText">
            <string>&amp;Redo</string>
        </property>
        <property name="accel">
            <string>Ctrl+Y</string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>editCutAction</cstring>
        </property>
        <property name="iconSet">
            <iconset></iconset>
        </property>
        <property name="text">
            <string>Cut</string>
        </property>
        <property name="menuText">
            <string>Cu&amp;t</string>
        </property>
        <property name="accel">
            <string>Ctrl+X</string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>editCopyAction</cstring>
        </property>
        <property name="iconSet">
            <iconset></iconset>
        </property>
        <property name="text">
            <string>Copy</string>
        </property>
        <property name="menuText">
            <string>&amp;Copy</string>
        </property>
        <property name="accel">
            <string>Ctrl+C</string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>editPasteAction</cstring>
        </property>
        <property name="iconSet">
            <iconset></iconset>
        </property>
        <property name="text">
            <string>Paste</string>
        </property>
        <property name="menuText">
            <string>&amp;Paste</string>
        </property>
        <property name="accel">
            <string>Ctrl+V</string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>editFindAction</cstring>
        </property>
        <property name="iconSet">
            <iconset></iconset>
        </property>
        <property name="text">
            <string>Find</string>
        </property>
        <property name="menuText">
            <string>&amp;Find...</string>
        </property>
        <property name="accel">
            <string>Ctrl+F</string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>helpContentsAction</cstring>
        </property>
        <property name="text">
            <string>Contents</string>
        </property>
        <property name="menuText">
            <string>&amp;Contents...</string>
        </property>
        <property name="accel">
            <string></string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>helpIndexAction</cstring>
        </property>
        <property name="text">
            <string>Index</string>
        </property>
        <property name="menuText">
            <string>&amp;Index...</string>
        </property>
        <property name="accel">
            <string></string>
        </property>
    </action>
    <action>
        <property name="name">
            <cstring>helpAboutAction</cstring>
        </property>
        <property name="text">
            <string>About</string>
        </property>
        <property name="menuText">
            <string>&amp;About</string>
        </property>
        <property name="accel">
            <string></string>
        </property>
    </action>
</actions>
<images>
    <image name="image0">
        <data format="XBM.GZ" length="79">789c534e494dcbcc4b554829cdcdad8c2fcf4c29c95030e0524611cd48cd4ccf28010a1797249664262b2467241641a592324b8aa363156c15aab914146aadb90067111b1f</data>
    </image>
</images>
<connections>
    <connection>
        <sender>fileNewAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>fileNew()</slot>
    </connection>
    <connection>
        <sender>fileOpenAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>fileOpen()</slot>
    </connection>
    <connection>
        <sender>fileSaveAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>fileSave()</slot>
    </connection>
    <connection>
        <sender>fileSaveAsAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>fileSaveAs()</slot>
    </connection>
    <connection>
        <sender>filePrintAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>filePrint()</slot>
    </connection>
    <connection>
        <sender>fileExitAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>fileExit()</slot>
    </connection>
    <connection>
        <sender>editUndoAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>editUndo()</slot>
    </connection>
    <connection>
        <sender>editRedoAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>editRedo()</slot>
    </connection>
    <connection>
        <sender>editCutAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>editCut()</slot>
    </connection>
    <connection>
        <sender>editCopyAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>editCopy()</slot>
    </connection>
    <connection>
        <sender>editPasteAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>editPaste()</slot>
    </connection>
    <connection>
        <sender>editFindAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>editFind()</slot>
    </connection>
    <connection>
        <sender>helpIndexAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>helpIndex()</slot>
    </connection>
    <connection>
        <sender>helpContentsAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>helpContents()</slot>
    </connection>
    <connection>
        <sender>helpAboutAction</sender>
        <signal>activated()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>helpAbout()</slot>
    </connection>
    <connection>
        <sender>ButtonDraw</sender>
        <signal>clicked()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>Draw()</slot>
    </connection>
    <connection>
        <sender>ButtonRandomize</sender>
        <signal>clicked()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>Randomize()</slot>
    </connection>
    <connection>
        <sender>ButtonRestore</sender>
        <signal>clicked()</signal>
        <receiver>QuasiStripesForm</receiver>
        <slot>Restore()</slot>
    </connection>
</connections>
<includes>
    <include location="local" impldecl="in implementation">quasistripesform.ui.h</include>
</includes>
<slots>
    <slot>fileNew()</slot>
    <slot>fileOpen()</slot>
    <slot>fileSave()</slot>
    <slot>fileSaveAs()</slot>
    <slot>filePrint()</slot>
    <slot>fileExit()</slot>
    <slot>editUndo()</slot>
    <slot>editRedo()</slot>
    <slot>editCut()</slot>
    <slot>editCopy()</slot>
    <slot>editPaste()</slot>
    <slot>editFind()</slot>
    <slot>helpIndex()</slot>
    <slot>helpContents()</slot>
    <slot>helpAbout()</slot>
    <slot>Draw()</slot>
    <slot>Randomize()</slot>
    <slot>Restore()</slot>
</slots>
<pixmapinproject/>
<layoutdefaults spacing="6" margin="11"/>
<includehints>
    <includehint>quasiperiodic_paintstripeswidget.h</includehint>
</includehints>
</UI>
