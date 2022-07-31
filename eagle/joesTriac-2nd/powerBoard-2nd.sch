<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.2.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="4" name="Route4" color="1" fill="4" visible="no" active="no"/>
<layer number="5" name="Route5" color="4" fill="4" visible="no" active="no"/>
<layer number="6" name="Route6" color="1" fill="8" visible="no" active="no"/>
<layer number="7" name="Route7" color="4" fill="8" visible="no" active="no"/>
<layer number="8" name="Route8" color="1" fill="2" visible="no" active="no"/>
<layer number="9" name="Route9" color="4" fill="2" visible="no" active="no"/>
<layer number="10" name="Route10" color="1" fill="7" visible="no" active="no"/>
<layer number="11" name="Route11" color="4" fill="7" visible="no" active="no"/>
<layer number="12" name="Route12" color="1" fill="5" visible="no" active="no"/>
<layer number="13" name="Route13" color="4" fill="5" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="trafo">
<description>&lt;b&gt;Transformers&lt;/b&gt;&lt;p&gt;
Based on the following previous libraries:
&lt;ul&gt;
&lt;li&gt;trafo-b.lbr: transformers from BLOCK
&lt;li&gt;trafo-e.lbr: transformers from ERA
&lt;li&gt;trafo-r.lbr: ring cores
&lt;/ul&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="EE20-2">
<description>&lt;b&gt;TRANSFORMER&lt;/b&gt;&lt;p&gt;
1 primary winding, 2 secondary windings</description>
<wire x1="-4.445" y1="3.81" x2="-4.445" y2="-3.81" width="0.254" layer="21"/>
<wire x1="-2.54" y1="3.81" x2="-2.54" y2="-3.81" width="0.254" layer="21"/>
<wire x1="-2.54" y1="-3.81" x2="-4.445" y2="-3.81" width="0.254" layer="21"/>
<wire x1="-1.27" y1="6.35" x2="-1.27" y2="-6.35" width="0.254" layer="21"/>
<wire x1="0" y1="6.35" x2="0" y2="1.016" width="0.254" layer="21"/>
<wire x1="0" y1="1.016" x2="1.905" y2="1.016" width="0.254" layer="21"/>
<wire x1="0" y1="6.35" x2="1.905" y2="6.35" width="0.254" layer="21"/>
<wire x1="1.905" y1="1.016" x2="1.905" y2="6.35" width="0.254" layer="21"/>
<wire x1="-4.445" y1="3.81" x2="-2.54" y2="3.81" width="0.254" layer="21"/>
<wire x1="-6.35" y1="3.81" x2="-4.445" y2="3.81" width="0.254" layer="21"/>
<wire x1="-4.445" y1="-3.81" x2="-6.35" y2="-3.81" width="0.254" layer="21"/>
<wire x1="1.905" y1="1.016" x2="3.81" y2="1.016" width="0.254" layer="21"/>
<wire x1="1.905" y1="6.35" x2="3.81" y2="6.35" width="0.254" layer="21"/>
<wire x1="-11.43" y1="-10.287" x2="-10.16" y2="-11.557" width="0.254" layer="21" curve="90"/>
<wire x1="10.16" y1="11.557" x2="11.43" y2="10.287" width="0.254" layer="21" curve="-90"/>
<wire x1="10.16" y1="-11.557" x2="11.43" y2="-10.287" width="0.254" layer="21" curve="90"/>
<wire x1="11.43" y1="10.287" x2="11.43" y2="-10.287" width="0.254" layer="21"/>
<wire x1="-11.43" y1="10.287" x2="-10.16" y2="11.557" width="0.254" layer="21" curve="-90"/>
<wire x1="-11.43" y1="-10.287" x2="-11.43" y2="10.287" width="0.254" layer="21"/>
<wire x1="10.16" y1="-7.62" x2="10.795" y2="-6.985" width="0.254" layer="21" curve="90"/>
<wire x1="-10.795" y1="-6.985" x2="-10.16" y2="-7.62" width="0.254" layer="21" curve="90"/>
<wire x1="10.16" y1="7.62" x2="10.795" y2="6.985" width="0.254" layer="21" curve="-90"/>
<wire x1="-10.795" y1="6.985" x2="-10.16" y2="7.62" width="0.254" layer="21" curve="-90"/>
<wire x1="10.16" y1="-11.557" x2="-10.16" y2="-11.557" width="0.254" layer="21"/>
<wire x1="-10.16" y1="11.557" x2="10.16" y2="11.557" width="0.254" layer="21"/>
<wire x1="-9.525" y1="-7.62" x2="-5.461" y2="-7.62" width="0.254" layer="51"/>
<wire x1="-9.525" y1="7.62" x2="-5.461" y2="7.62" width="0.254" layer="51"/>
<wire x1="-10.795" y1="6.985" x2="-10.795" y2="-6.985" width="0.254" layer="21"/>
<wire x1="9.525" y1="7.62" x2="5.461" y2="7.62" width="0.254" layer="51"/>
<wire x1="10.795" y1="-6.985" x2="10.795" y2="6.985" width="0.254" layer="21"/>
<wire x1="9.525" y1="-7.62" x2="5.461" y2="-7.62" width="0.254" layer="51"/>
<wire x1="5.461" y1="7.62" x2="-5.461" y2="7.62" width="0.254" layer="21"/>
<wire x1="5.461" y1="-7.62" x2="-5.461" y2="-7.62" width="0.254" layer="21"/>
<wire x1="0" y1="-1.016" x2="0" y2="-6.35" width="0.254" layer="21"/>
<wire x1="0" y1="-6.35" x2="1.905" y2="-6.35" width="0.254" layer="21"/>
<wire x1="0" y1="-1.016" x2="1.905" y2="-1.016" width="0.254" layer="21"/>
<wire x1="1.905" y1="-6.35" x2="1.905" y2="-1.016" width="0.254" layer="21"/>
<wire x1="1.905" y1="-6.35" x2="3.81" y2="-6.35" width="0.254" layer="21"/>
<wire x1="1.905" y1="-1.016" x2="3.81" y2="-1.016" width="0.254" layer="21"/>
<wire x1="10.16" y1="-7.62" x2="9.525" y2="-7.62" width="0.254" layer="21"/>
<wire x1="10.16" y1="7.62" x2="9.525" y2="7.62" width="0.254" layer="21"/>
<wire x1="-10.16" y1="7.62" x2="-9.525" y2="7.62" width="0.254" layer="21"/>
<wire x1="-10.16" y1="-7.62" x2="-9.525" y2="-7.62" width="0.254" layer="21"/>
<circle x="-5.207" y="-3.048" radius="0.254" width="0.254" layer="21"/>
<circle x="2.667" y="1.778" radius="0.254" width="0.254" layer="21"/>
<circle x="-6.985" y="3.81" radius="0.635" width="0.254" layer="21"/>
<circle x="-6.985" y="-3.81" radius="0.635" width="0.254" layer="21"/>
<circle x="4.445" y="1.016" radius="0.635" width="0.254" layer="21"/>
<circle x="4.445" y="6.35" radius="0.635" width="0.254" layer="21"/>
<circle x="2.667" y="-5.588" radius="0.254" width="0.254" layer="21"/>
<circle x="4.445" y="-6.35" radius="0.635" width="0.254" layer="21"/>
<circle x="4.445" y="-1.016" radius="0.635" width="0.254" layer="21"/>
<pad name="1" x="-7.493" y="7.493" drill="1.016" shape="long" rot="R90"/>
<pad name="4" x="-7.493" y="-7.493" drill="1.016" shape="long" rot="R90"/>
<pad name="5" x="7.493" y="-7.493" drill="1.016" shape="long"/>
<pad name="6" x="7.493" y="-2.5146" drill="1.016" shape="long"/>
<pad name="7" x="7.493" y="2.5146" drill="1.016" shape="long"/>
<pad name="8" x="7.493" y="7.493" drill="1.016" shape="long"/>
<text x="-10.16" y="12.065" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.27" y="12.065" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="-8.001" y="-0.889" size="1.778" layer="21" ratio="10">P1</text>
<text x="-4.699" y="-10.922" size="1.778" layer="21" ratio="10">EE20</text>
<text x="5.207" y="-5.334" size="1.778" layer="21" ratio="10" rot="R90">S1</text>
<text x="5.207" y="2.286" size="1.778" layer="21" ratio="10" rot="R90">S2</text>
<text x="-10.16" y="8.89" size="1.27" layer="21" ratio="10">1</text>
<text x="-10.16" y="-10.16" size="1.27" layer="21" ratio="10">4</text>
<text x="9.525" y="-10.16" size="1.27" layer="21" ratio="10">5</text>
<text x="9.525" y="8.89" size="1.27" layer="21" ratio="10">8</text>
<rectangle x1="-4.445" y1="-3.81" x2="-2.54" y2="3.81" layer="21"/>
<rectangle x1="0" y1="-6.35" x2="1.905" y2="-1.016" layer="21"/>
<rectangle x1="0" y1="1.016" x2="1.905" y2="6.35" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="T1P2S">
<wire x1="1.27" y1="5.715" x2="1.27" y2="-8.255" width="0.254" layer="94"/>
<circle x="-2.032" y="-8.128" radius="0.127" width="0.3048" layer="94"/>
<circle x="4.699" y="-8.128" radius="0.127" width="0.3048" layer="94"/>
<circle x="4.699" y="-0.508" radius="0.127" width="0.3048" layer="94"/>
<text x="-2.54" y="7.366" size="1.778" layer="95">&gt;NAME</text>
<text x="-2.54" y="-10.795" size="1.778" layer="96">&gt;VALUE</text>
<text x="-3.175" y="-2.54" size="1.27" layer="94" rot="R90">PRI</text>
<text x="6.477" y="-6.223" size="1.27" layer="94" rot="R90">S1</text>
<text x="6.477" y="1.397" size="1.27" layer="94" rot="R90">S2</text>
<rectangle x1="-2.286" y1="-7.62" x2="0.254" y2="5.08" layer="94"/>
<rectangle x1="2.286" y1="0" x2="4.826" y2="5.08" layer="94"/>
<rectangle x1="2.286" y1="-7.62" x2="4.826" y2="-2.54" layer="94"/>
<pin name="1" x="-5.08" y="5.08" visible="pad" length="middle" direction="pas"/>
<pin name="2" x="-5.08" y="-7.62" visible="pad" length="middle" direction="pas"/>
<pin name="3" x="7.62" y="-7.62" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="4" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="5" x="7.62" y="0" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="6" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="EE20-2" prefix="TR" uservalue="yes">
<description>&lt;b&gt;TRANSFORMER&lt;/b&gt;&lt;p&gt;
1 primary winding, 2 secondary windings&lt;p&gt;
ERA</description>
<gates>
<gate name="1" symbol="T1P2S" x="0" y="0"/>
</gates>
<devices>
<device name="" package="EE20-2">
<connects>
<connect gate="1" pin="1" pad="1"/>
<connect gate="1" pin="2" pad="4"/>
<connect gate="1" pin="3" pad="5"/>
<connect gate="1" pin="4" pad="6"/>
<connect gate="1" pin="5" pad="7"/>
<connect gate="1" pin="6" pad="8"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="con-omron">
<description>&lt;b&gt;OMRON FPC Connectors&lt;/b&gt;&lt;p&gt;
Original backlock mechanism ensures greater work efficiency and higher reliability.&lt;br&gt;
Source : http://www.omron.com/ecb/products/pdf/fpc.pdf&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="XF2E-0815-1">
<description>&lt;b&gt;Non-ZIF Type (0.8 mm-pitch)&lt;/b&gt;&lt;p&gt;
Source : http://www.omron.com/ecb/products/pdf/fpc.pdf</description>
<wire x1="1.99" y1="2.45" x2="1.69" y2="2.6" width="0.1016" layer="21"/>
<wire x1="1.69" y1="2.6" x2="1.69" y2="3" width="0.1016" layer="21"/>
<wire x1="1.69" y1="3" x2="1.99" y2="3.15" width="0.1016" layer="21"/>
<wire x1="1.99" y1="2.45" x2="1.99" y2="2.35" width="0.1016" layer="21"/>
<wire x1="1.99" y1="2.35" x2="1.69" y2="2.2" width="0.1016" layer="21"/>
<wire x1="1.69" y1="2.2" x2="1.69" y2="1.8" width="0.1016" layer="21"/>
<wire x1="1.69" y1="1.8" x2="1.99" y2="1.65" width="0.1016" layer="21"/>
<wire x1="1.99" y1="1.65" x2="1.99" y2="1.55" width="0.1016" layer="21"/>
<wire x1="1.99" y1="1.55" x2="1.69" y2="1.4" width="0.1016" layer="21"/>
<wire x1="1.99" y1="0.85" x2="1.69" y2="1" width="0.1016" layer="21"/>
<wire x1="1.69" y1="1" x2="1.69" y2="1.4" width="0.1016" layer="21"/>
<wire x1="1.99" y1="0.85" x2="1.99" y2="0.75" width="0.1016" layer="21"/>
<wire x1="1.99" y1="0.75" x2="1.69" y2="0.6" width="0.1016" layer="21"/>
<wire x1="1.69" y1="0.6" x2="1.69" y2="0.2" width="0.1016" layer="21"/>
<wire x1="1.69" y1="0.2" x2="1.99" y2="0.05" width="0.1016" layer="21"/>
<wire x1="1.99" y1="0.05" x2="1.99" y2="-0.05" width="0.1016" layer="21"/>
<wire x1="1.99" y1="-0.05" x2="1.69" y2="-0.2" width="0.1016" layer="21"/>
<wire x1="1.99" y1="-0.75" x2="1.69" y2="-0.6" width="0.1016" layer="21"/>
<wire x1="1.69" y1="-0.6" x2="1.69" y2="-0.2" width="0.1016" layer="21"/>
<wire x1="1.99" y1="-0.75" x2="1.99" y2="-0.85" width="0.1016" layer="21"/>
<wire x1="1.99" y1="-0.85" x2="1.69" y2="-1" width="0.1016" layer="21"/>
<wire x1="1.69" y1="-1" x2="1.69" y2="-1.4" width="0.1016" layer="21"/>
<wire x1="1.69" y1="-1.4" x2="1.99" y2="-1.55" width="0.1016" layer="21"/>
<wire x1="1.99" y1="-1.55" x2="1.99" y2="-1.65" width="0.1016" layer="21"/>
<wire x1="1.99" y1="-1.65" x2="1.69" y2="-1.8" width="0.1016" layer="21"/>
<wire x1="1.99" y1="-2.35" x2="1.69" y2="-2.2" width="0.1016" layer="21"/>
<wire x1="1.69" y1="-2.2" x2="1.69" y2="-1.8" width="0.1016" layer="21"/>
<wire x1="1.99" y1="-2.35" x2="1.99" y2="-2.45" width="0.1016" layer="21"/>
<wire x1="1.99" y1="-2.45" x2="1.69" y2="-2.6" width="0.1016" layer="21"/>
<wire x1="1.69" y1="-2.6" x2="1.69" y2="-3" width="0.1016" layer="21"/>
<wire x1="1.69" y1="-3" x2="1.99" y2="-3.15" width="0.1016" layer="21"/>
<wire x1="1.99" y1="3.15" x2="1.99" y2="3.75" width="0.1016" layer="21"/>
<wire x1="1.99" y1="3.75" x2="0.99" y2="3.75" width="0.1016" layer="21"/>
<wire x1="0.99" y1="3.75" x2="0.99" y2="4.95" width="0.1016" layer="21"/>
<wire x1="0.99" y1="4.95" x2="-1.46" y2="4.95" width="0.1016" layer="21"/>
<wire x1="-1.46" y1="4.95" x2="-1.46" y2="4.8" width="0.1016" layer="21"/>
<wire x1="-1.46" y1="4.8" x2="-1.36" y2="4.7" width="0.1016" layer="21"/>
<wire x1="-1.36" y1="4.7" x2="-2.81" y2="4.7" width="0.1016" layer="21"/>
<wire x1="-2.81" y1="4.7" x2="-2.81" y2="4.15" width="0.1016" layer="51"/>
<wire x1="-2.81" y1="4.15" x2="-2.61" y2="4.15" width="0.1016" layer="51"/>
<wire x1="-2.61" y1="4.15" x2="-2.81" y2="3.9" width="0.1016" layer="51"/>
<wire x1="-2.81" y1="3.9" x2="-2.81" y2="-3.9" width="0.1016" layer="21"/>
<wire x1="-2.81" y1="-4.15" x2="-2.81" y2="-4.7" width="0.1016" layer="51"/>
<wire x1="-1.36" y1="-4.7" x2="-2.81" y2="-4.7" width="0.1016" layer="21"/>
<wire x1="-1.36" y1="-4.7" x2="-1.46" y2="-4.8" width="0.1016" layer="21"/>
<wire x1="-1.46" y1="-4.8" x2="-1.46" y2="-4.95" width="0.1016" layer="21"/>
<wire x1="0.99" y1="-4.95" x2="-1.46" y2="-4.95" width="0.1016" layer="21"/>
<wire x1="-2.81" y1="-4.15" x2="-2.61" y2="-4.15" width="0.1016" layer="51"/>
<wire x1="-2.81" y1="-3.9" x2="-2.61" y2="-4.15" width="0.1016" layer="51"/>
<wire x1="0.99" y1="-4.95" x2="0.99" y2="-3.75" width="0.1016" layer="21"/>
<wire x1="0.99" y1="-3.75" x2="1.99" y2="-3.75" width="0.1016" layer="21"/>
<wire x1="1.99" y1="-3.75" x2="1.99" y2="-3.15" width="0.1016" layer="21"/>
<smd name="1" x="2.54" y="2.8" dx="1.4" dy="0.4" layer="1"/>
<smd name="2" x="2.54" y="2" dx="1.4" dy="0.4" layer="1"/>
<smd name="3" x="2.54" y="1.2" dx="1.4" dy="0.4" layer="1"/>
<smd name="4" x="2.54" y="0.4" dx="1.4" dy="0.4" layer="1"/>
<smd name="5" x="2.54" y="-0.4" dx="1.4" dy="0.4" layer="1"/>
<smd name="6" x="2.54" y="-1.2" dx="1.4" dy="0.4" layer="1"/>
<smd name="7" x="2.54" y="-2" dx="1.4" dy="0.4" layer="1"/>
<smd name="8" x="2.54" y="-2.8" dx="1.4" dy="0.4" layer="1"/>
<smd name="M1" x="-2.11" y="4.3" dx="2.1" dy="0.6" layer="1"/>
<smd name="M2" x="-2.11" y="-4.3" dx="2.1" dy="0.6" layer="1"/>
<text x="-2.8" y="5.6" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.8" y="-6.8" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="1.74" y1="2.7" x2="2.74" y2="2.9" layer="51"/>
<rectangle x1="1.74" y1="1.9" x2="2.74" y2="2.1" layer="51"/>
<rectangle x1="1.74" y1="1.1" x2="2.74" y2="1.3" layer="51"/>
<rectangle x1="1.74" y1="0.3" x2="2.74" y2="0.5" layer="51"/>
<rectangle x1="1.74" y1="-0.5" x2="2.74" y2="-0.3" layer="51"/>
<rectangle x1="1.74" y1="-1.3" x2="2.74" y2="-1.1" layer="51"/>
<rectangle x1="1.74" y1="-2.1" x2="2.74" y2="-1.9" layer="51"/>
<rectangle x1="1.74" y1="-2.9" x2="2.74" y2="-2.7" layer="51"/>
</package>
<package name="XF2J-0824-11">
<description>&lt;b&gt;ZIF Slide-locking Type (0.5 mm-pitch) &lt;/b&gt; Standard&lt;p&gt;
Source : http://www.omron.com/ecb/products/pdf/fpc.pdf</description>
<wire x1="1.2" y1="3.9" x2="0.9" y2="4.2" width="0.1016" layer="51" curve="90"/>
<wire x1="0.9" y1="4.2" x2="-1.35" y2="4.2" width="0.1016" layer="51"/>
<wire x1="-1.35" y1="4.2" x2="-1.65" y2="3.9" width="0.1016" layer="51" curve="90"/>
<wire x1="-1.65" y1="3.9" x2="-1.65" y2="-3.9" width="0.1016" layer="51"/>
<wire x1="-1.65" y1="-3.9" x2="-1.35" y2="-4.2" width="0.1016" layer="51" curve="90"/>
<wire x1="-1.35" y1="-4.2" x2="0.9" y2="-4.2" width="0.1016" layer="51"/>
<wire x1="0.9" y1="-4.2" x2="1.2" y2="-3.9" width="0.1016" layer="51" curve="90"/>
<wire x1="1.2" y1="-3.9" x2="1.2" y2="3.9" width="0.1016" layer="51"/>
<wire x1="-1.625" y1="2.25" x2="-1.15" y2="2.05" width="0.1016" layer="51"/>
<wire x1="-1.15" y1="2.05" x2="-1.15" y2="-2.05" width="0.1016" layer="51"/>
<wire x1="-1.15" y1="-2.05" x2="-1.625" y2="-2.25" width="0.1016" layer="51"/>
<wire x1="1.35" y1="-4.2" x2="1.65" y2="-3.9" width="0.1016" layer="51" curve="90"/>
<wire x1="1.65" y1="-3.9" x2="1.65" y2="3.9" width="0.1016" layer="51"/>
<wire x1="1.65" y1="3.9" x2="1.35" y2="4.2" width="0.1016" layer="51" curve="90"/>
<wire x1="1" y1="4.2" x2="1.35" y2="4.2" width="0.1016" layer="51"/>
<wire x1="0.975" y1="-4.2" x2="1.35" y2="-4.2" width="0.1016" layer="51"/>
<wire x1="1.15" y1="1.95" x2="1" y2="1.95" width="0.1016" layer="51"/>
<wire x1="1" y1="1.55" x2="1.15" y2="1.55" width="0.1016" layer="51"/>
<wire x1="1" y1="1.95" x2="1" y2="1.55" width="0.1016" layer="51"/>
<wire x1="-1.625" y1="2.25" x2="-1.15" y2="2.05" width="0.1016" layer="21"/>
<wire x1="-1.65" y1="3.9" x2="-1.65" y2="2.1" width="0.1016" layer="21"/>
<wire x1="1.65" y1="2.6" x2="1.65" y2="3.9" width="0.1016" layer="21"/>
<wire x1="1.65" y1="-3.9" x2="1.65" y2="-2.1" width="0.1016" layer="21"/>
<wire x1="-1.65" y1="-2.6" x2="-1.65" y2="-3.9" width="0.1016" layer="21"/>
<smd name="1" x="1.2" y="1.75" dx="0.5" dy="1.6" layer="1" rot="R270"/>
<smd name="2" x="-1.2" y="1.25" dx="0.5" dy="1.6" layer="1" rot="R90"/>
<smd name="3" x="1.2" y="0.75" dx="0.5" dy="1.6" layer="1" rot="R270"/>
<smd name="4" x="-1.2" y="0.25" dx="0.5" dy="1.6" layer="1" rot="R90"/>
<smd name="5" x="1.2" y="-0.25" dx="0.5" dy="1.6" layer="1" rot="R270"/>
<smd name="6" x="-1.2" y="-0.75" dx="0.5" dy="1.6" layer="1" rot="R90"/>
<smd name="7" x="1.2" y="-1.25" dx="0.5" dy="1.6" layer="1" rot="R270"/>
<smd name="8" x="-1.2" y="-1.75" dx="0.5" dy="1.6" layer="1" rot="R90"/>
<smd name="M1" x="0" y="3.45" dx="1.4" dy="2.9" layer="1" rot="R270"/>
<smd name="M2" x="0" y="-3.45" dx="1.4" dy="2.9" layer="1" rot="R270"/>
<text x="-2" y="4.5" size="1.27" layer="25">&gt;NAME</text>
<text x="-2" y="-6" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="XF2J-0824-12">
<description>&lt;b&gt;ZIF Slide-locking Type (0.5 mm-pitch) &lt;/b&gt; Reverse&lt;p&gt;
Source : http://www.omron.com/ecb/products/pdf/fpc.pdf</description>
<wire x1="1.2" y1="3.9" x2="0.9" y2="4.2" width="0.1016" layer="51" curve="90"/>
<wire x1="0.9" y1="4.2" x2="-1.35" y2="4.2" width="0.1016" layer="51"/>
<wire x1="-1.35" y1="4.2" x2="-1.65" y2="3.9" width="0.1016" layer="51" curve="90"/>
<wire x1="-1.65" y1="3.9" x2="-1.65" y2="-3.9" width="0.1016" layer="51"/>
<wire x1="-1.65" y1="-3.9" x2="-1.35" y2="-4.2" width="0.1016" layer="51" curve="90"/>
<wire x1="-1.35" y1="-4.2" x2="0.9" y2="-4.2" width="0.1016" layer="51"/>
<wire x1="0.9" y1="-4.2" x2="1.2" y2="-3.9" width="0.1016" layer="51" curve="90"/>
<wire x1="1.2" y1="-3.9" x2="1.2" y2="3.9" width="0.1016" layer="51"/>
<wire x1="-1.625" y1="2.25" x2="-1.15" y2="2.05" width="0.1016" layer="51"/>
<wire x1="-1.15" y1="2.05" x2="-1.15" y2="-2.05" width="0.1016" layer="51"/>
<wire x1="-1.15" y1="-2.05" x2="-1.625" y2="-2.25" width="0.1016" layer="51"/>
<wire x1="1.35" y1="-4.2" x2="1.65" y2="-3.9" width="0.1016" layer="51" curve="90"/>
<wire x1="1.65" y1="-3.9" x2="1.65" y2="3.9" width="0.1016" layer="51"/>
<wire x1="1.65" y1="3.9" x2="1.35" y2="4.2" width="0.1016" layer="51" curve="90"/>
<wire x1="1" y1="4.2" x2="1.35" y2="4.2" width="0.1016" layer="51"/>
<wire x1="0.975" y1="-4.2" x2="1.35" y2="-4.2" width="0.1016" layer="51"/>
<wire x1="1.15" y1="1.95" x2="1" y2="1.95" width="0.1016" layer="51"/>
<wire x1="1" y1="1.55" x2="1.15" y2="1.55" width="0.1016" layer="51"/>
<wire x1="1" y1="1.95" x2="1" y2="1.55" width="0.1016" layer="51"/>
<wire x1="-1.625" y1="2.25" x2="-1.15" y2="2.05" width="0.1016" layer="21"/>
<wire x1="-1.65" y1="3.9" x2="-1.65" y2="2.1" width="0.1016" layer="21"/>
<wire x1="1.65" y1="2.6" x2="1.65" y2="3.9" width="0.1016" layer="21"/>
<wire x1="1.65" y1="-3.9" x2="1.65" y2="-2.1" width="0.1016" layer="21"/>
<wire x1="-1.65" y1="-2.6" x2="-1.65" y2="-3.9" width="0.1016" layer="21"/>
<smd name="1" x="-1.2" y="1.75" dx="0.5" dy="1.6" layer="1" rot="R270"/>
<smd name="2" x="1.2" y="1.25" dx="0.5" dy="1.6" layer="1" rot="R90"/>
<smd name="3" x="-1.2" y="0.75" dx="0.5" dy="1.6" layer="1" rot="R270"/>
<smd name="4" x="1.2" y="0.25" dx="0.5" dy="1.6" layer="1" rot="R90"/>
<smd name="5" x="-1.2" y="-0.25" dx="0.5" dy="1.6" layer="1" rot="R270"/>
<smd name="6" x="1.2" y="-0.75" dx="0.5" dy="1.6" layer="1" rot="R90"/>
<smd name="7" x="-1.2" y="-1.25" dx="0.5" dy="1.6" layer="1" rot="R270"/>
<smd name="8" x="1.2" y="-1.75" dx="0.5" dy="1.6" layer="1" rot="R90"/>
<smd name="M1" x="0" y="3.45" dx="1.4" dy="2.9" layer="1" rot="R270"/>
<smd name="M2" x="0" y="-3.45" dx="1.4" dy="2.9" layer="1" rot="R270"/>
<text x="-2" y="4.5" size="1.27" layer="25">&gt;NAME</text>
<text x="-2" y="-6" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="XF2L-0825-1">
<description>&lt;b&gt;ZIF Slide-locking Type (0.5 mm-pitch)&lt;/b&gt; Upper-contact Type&lt;p&gt;
Source : http://www.omron.com/ecb/products/pdf/fpc.pdf</description>
<wire x1="-1.495" y1="-4.35" x2="-1.3649" y2="-4.4851" width="0.1016" layer="21"/>
<wire x1="-1.3649" y1="-4.4851" x2="-0.9385" y2="-4.4851" width="0.1016" layer="21"/>
<wire x1="-0.9385" y1="-4.4851" x2="-0.801" y2="-4.3476" width="0.1016" layer="21"/>
<wire x1="-0.801" y1="-4.3476" x2="-0.801" y2="-3.6557" width="0.1016" layer="21"/>
<wire x1="-0.801" y1="-3.6557" x2="-0.7536" y2="-3.5562" width="0.1016" layer="21" curve="-50.916048"/>
<wire x1="-0.7536" y1="-3.5562" x2="-0.5403" y2="-3.4804" width="0.1016" layer="21" curve="-39.080411"/>
<wire x1="-0.5403" y1="-3.4804" x2="-0.5403" y2="-4.4329" width="0.1016" layer="21"/>
<wire x1="-0.5403" y1="-4.4329" x2="0.5117" y2="-4.4329" width="0.1016" layer="51"/>
<wire x1="0.5117" y1="-4.4329" x2="0.5117" y2="-3.9496" width="0.1016" layer="51"/>
<wire x1="0.5117" y1="-3.9496" x2="-0.019" y2="-3.9496" width="0.1016" layer="51"/>
<wire x1="-0.019" y1="-3.9496" x2="-0.019" y2="-3.8027" width="0.1016" layer="51"/>
<wire x1="-0.019" y1="-3.8027" x2="0.6302" y2="-3.8027" width="0.1016" layer="51"/>
<wire x1="0.6302" y1="-3.8027" x2="0.6302" y2="-3.4567" width="0.1016" layer="51"/>
<wire x1="0.6302" y1="-3.4567" x2="0.8577" y2="-3.4567" width="0.1016" layer="51"/>
<wire x1="0.8577" y1="-3.4567" x2="0.8577" y2="-3.9401" width="0.1016" layer="51"/>
<wire x1="0.8577" y1="-3.9401" x2="1.5591" y2="-3.9401" width="0.1016" layer="51"/>
<wire x1="1.5591" y1="-3.9401" x2="1.5591" y2="-2.2293" width="0.1016" layer="21"/>
<wire x1="1.5591" y1="-2.2293" x2="1.2131" y2="-2.2293" width="0.1016" layer="21"/>
<wire x1="-1.495" y1="-4.35" x2="-1.495" y2="4.35" width="0.1016" layer="21"/>
<wire x1="-1.495" y1="4.35" x2="-1.3649" y2="4.4851" width="0.1016" layer="21"/>
<wire x1="-1.3649" y1="4.4851" x2="-0.9385" y2="4.4851" width="0.1016" layer="21"/>
<wire x1="-0.9385" y1="4.4851" x2="-0.801" y2="4.3476" width="0.1016" layer="21"/>
<wire x1="-0.801" y1="4.3476" x2="-0.801" y2="3.6557" width="0.1016" layer="21"/>
<wire x1="-0.801" y1="3.6557" x2="-0.7536" y2="3.5562" width="0.1016" layer="21" curve="50.916048"/>
<wire x1="-0.7536" y1="3.5562" x2="-0.5403" y2="3.4804" width="0.1016" layer="21" curve="39.080411"/>
<wire x1="-0.5403" y1="3.4804" x2="-0.5403" y2="4.4329" width="0.1016" layer="51"/>
<wire x1="-0.5403" y1="4.4329" x2="0.5117" y2="4.4329" width="0.1016" layer="51"/>
<wire x1="0.5117" y1="4.4329" x2="0.5117" y2="3.9496" width="0.1016" layer="51"/>
<wire x1="0.5117" y1="3.9496" x2="-0.019" y2="3.9496" width="0.1016" layer="51"/>
<wire x1="-0.019" y1="3.9496" x2="-0.019" y2="3.8027" width="0.1016" layer="51"/>
<wire x1="-0.019" y1="3.8027" x2="0.6302" y2="3.8027" width="0.1016" layer="51"/>
<wire x1="0.6302" y1="3.8027" x2="0.6302" y2="3.4567" width="0.1016" layer="51"/>
<wire x1="0.6302" y1="3.4567" x2="0.8577" y2="3.4567" width="0.1016" layer="51"/>
<wire x1="0.8577" y1="3.4567" x2="0.8577" y2="3.9401" width="0.1016" layer="51"/>
<wire x1="0.8577" y1="3.9401" x2="1.5591" y2="3.9401" width="0.1016" layer="51"/>
<wire x1="1.5591" y1="3.9401" x2="1.5591" y2="2.2293" width="0.1016" layer="21"/>
<wire x1="1.538" y1="2.2293" x2="1.2131" y2="2.2293" width="0.1016" layer="21"/>
<wire x1="1.2131" y1="-2.2293" x2="1.2131" y2="2.2293" width="0.1016" layer="21"/>
<wire x1="-1.4407" y1="3.6226" x2="-1.0047" y2="3.3667" width="0.1016" layer="25"/>
<wire x1="-1.0047" y1="3.3667" x2="-1.4407" y2="3.0823" width="0.1016" layer="25"/>
<wire x1="-2.995" y1="-4.35" x2="-2.8649" y2="-4.4851" width="0.1016" layer="21"/>
<wire x1="-2.8649" y1="-4.4851" x2="-2.4385" y2="-4.4851" width="0.1016" layer="21" style="shortdash"/>
<wire x1="-2.995" y1="-4.35" x2="-2.995" y2="4.35" width="0.1016" layer="21" style="shortdash"/>
<wire x1="-2.995" y1="4.35" x2="-2.8649" y2="4.4851" width="0.1016" layer="21"/>
<wire x1="-2.8649" y1="4.4851" x2="-2.4385" y2="4.4851" width="0.1016" layer="21" style="shortdash"/>
<wire x1="-2.4385" y1="4.4851" x2="-2.301" y2="4.3476" width="0.1016" layer="21"/>
<wire x1="-2.4385" y1="-4.4851" x2="-2.301" y2="-4.3476" width="0.1016" layer="21"/>
<wire x1="-2.301" y1="-4.3476" x2="-2.301" y2="-3.6557" width="0.1016" layer="21" style="shortdash"/>
<wire x1="-2.301" y1="4.3476" x2="-2.301" y2="3.6557" width="0.1016" layer="21" style="shortdash"/>
<wire x1="-2.2536" y1="3.5562" x2="-2.0403" y2="3.4804" width="0.1016" layer="21" curve="39.080411"/>
<wire x1="-2.2536" y1="-3.5562" x2="-2.0403" y2="-3.4804" width="0.1016" layer="21" curve="-39.080411"/>
<wire x1="-2.301" y1="-3.6557" x2="-2.2536" y2="-3.5562" width="0.1016" layer="21" curve="-50.916048"/>
<wire x1="-2.301" y1="3.6557" x2="-2.2536" y2="3.5562" width="0.1016" layer="21" curve="50.916048"/>
<wire x1="-1.245" y1="-2.1" x2="-1.245" y2="2.1" width="0.1016" layer="21"/>
<wire x1="-1.495" y1="2.15" x2="-1.245" y2="2.1" width="0.1016" layer="21"/>
<wire x1="-1.245" y1="-2.1" x2="-1.495" y2="-2.15" width="0.1016" layer="21"/>
<wire x1="-2.745" y1="-2.1" x2="-2.745" y2="2.1" width="0.1016" layer="21" style="shortdash"/>
<wire x1="-2.995" y1="2.15" x2="-2.745" y2="2.1" width="0.1016" layer="21"/>
<wire x1="-2.745" y1="-2.1" x2="-2.995" y2="-2.15" width="0.1016" layer="21"/>
<circle x="-1.3412" y="3.3667" radius="0.0948" width="0" layer="25"/>
<smd name="1" x="1.905" y="1.75" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="2" x="1.905" y="1.25" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="3" x="1.905" y="0.75" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="4" x="1.905" y="0.25" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="5" x="1.905" y="-0.25" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="6" x="1.905" y="-0.75" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="7" x="1.905" y="-1.25" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="8" x="1.905" y="-1.75" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="M1" x="0.18" y="4.1375" dx="1.34" dy="1" layer="1"/>
<smd name="M2" x="0.18" y="-4.1375" dx="1.34" dy="1" layer="1"/>
<text x="-1.5" y="5" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.5" y="-6.5" size="1.27" layer="27">&gt;VALUE</text>
<text x="-0.5" y="3" size="0.4064" layer="51" font="vector" rot="SR270">Upper-contact</text>
<rectangle x1="1.355" y1="1.575" x2="2.455" y2="1.925" layer="29"/>
<rectangle x1="1.355" y1="1.075" x2="2.455" y2="1.425" layer="29"/>
<rectangle x1="1.355" y1="0.575" x2="2.455" y2="0.925" layer="29"/>
<rectangle x1="1.355" y1="0.075" x2="2.455" y2="0.425" layer="29"/>
<rectangle x1="1.355" y1="-0.425" x2="2.455" y2="-0.075" layer="29"/>
<rectangle x1="1.355" y1="-0.925" x2="2.455" y2="-0.575" layer="29"/>
<rectangle x1="1.355" y1="-1.425" x2="2.455" y2="-1.075" layer="29"/>
<rectangle x1="1.355" y1="-1.925" x2="2.455" y2="-1.575" layer="29"/>
<rectangle x1="0.105" y1="-4.45" x2="0.805" y2="-3.925" layer="51"/>
<rectangle x1="-0.445" y1="-4.45" x2="0.105" y2="-3.65" layer="51"/>
<rectangle x1="0.105" y1="-4.45" x2="0.805" y2="-3.925" layer="51"/>
<rectangle x1="-0.445" y1="-4.45" x2="0.105" y2="-3.65" layer="51"/>
<rectangle x1="0.105" y1="3.925" x2="0.805" y2="4.45" layer="51"/>
<rectangle x1="-0.445" y1="3.65" x2="0.105" y2="4.45" layer="51"/>
<rectangle x1="1.255" y1="1.65" x2="1.98" y2="1.85" layer="51"/>
<rectangle x1="1.255" y1="1.15" x2="1.98" y2="1.35" layer="51"/>
<rectangle x1="1.255" y1="0.65" x2="1.98" y2="0.85" layer="51"/>
<rectangle x1="1.255" y1="0.15" x2="1.98" y2="0.35" layer="51"/>
<rectangle x1="1.255" y1="-0.35" x2="1.98" y2="-0.15" layer="51"/>
<rectangle x1="1.255" y1="-0.85" x2="1.98" y2="-0.65" layer="51"/>
<rectangle x1="1.255" y1="-1.35" x2="1.98" y2="-1.15" layer="51"/>
<rectangle x1="1.255" y1="-1.85" x2="1.98" y2="-1.65" layer="51"/>
</package>
<package name="XF2L-0835-1">
<description>&lt;b&gt;ZIF Slide-locking Type (0.5 mm-pitch)&lt;/b&gt; Lower-contact Type&lt;p&gt;
Source : http://www.omron.com/ecb/products/pdf/fpc.pdf</description>
<wire x1="-1.495" y1="-4.35" x2="-1.3649" y2="-4.4851" width="0.1016" layer="21"/>
<wire x1="-1.3649" y1="-4.4851" x2="-0.9385" y2="-4.4851" width="0.1016" layer="21"/>
<wire x1="-0.9385" y1="-4.4851" x2="-0.801" y2="-4.3476" width="0.1016" layer="21"/>
<wire x1="-0.801" y1="-4.3476" x2="-0.801" y2="-3.6557" width="0.1016" layer="21"/>
<wire x1="-0.801" y1="-3.6557" x2="-0.7536" y2="-3.5562" width="0.1016" layer="21" curve="-50.916048"/>
<wire x1="-0.7536" y1="-3.5562" x2="-0.5403" y2="-3.4804" width="0.1016" layer="21" curve="-39.080411"/>
<wire x1="-0.5403" y1="-3.4804" x2="-0.5403" y2="-4.4329" width="0.1016" layer="21"/>
<wire x1="-0.5403" y1="-4.4329" x2="0.5117" y2="-4.4329" width="0.1016" layer="51"/>
<wire x1="0.5117" y1="-4.4329" x2="0.5117" y2="-3.9496" width="0.1016" layer="51"/>
<wire x1="0.5117" y1="-3.9496" x2="-0.019" y2="-3.9496" width="0.1016" layer="51"/>
<wire x1="-0.019" y1="-3.9496" x2="-0.019" y2="-3.8027" width="0.1016" layer="51"/>
<wire x1="-0.019" y1="-3.8027" x2="0.6302" y2="-3.8027" width="0.1016" layer="51"/>
<wire x1="0.6302" y1="-3.8027" x2="0.6302" y2="-3.4567" width="0.1016" layer="51"/>
<wire x1="0.6302" y1="-3.4567" x2="0.8577" y2="-3.4567" width="0.1016" layer="51"/>
<wire x1="0.8577" y1="-3.4567" x2="0.8577" y2="-3.9401" width="0.1016" layer="51"/>
<wire x1="0.8577" y1="-3.9401" x2="1.5591" y2="-3.9401" width="0.1016" layer="51"/>
<wire x1="1.5591" y1="-3.9401" x2="1.5591" y2="-2.2293" width="0.1016" layer="21"/>
<wire x1="1.5591" y1="-2.2293" x2="1.2131" y2="-2.2293" width="0.1016" layer="21"/>
<wire x1="-1.495" y1="-4.35" x2="-1.495" y2="4.35" width="0.1016" layer="21"/>
<wire x1="-1.495" y1="4.35" x2="-1.3649" y2="4.4851" width="0.1016" layer="21"/>
<wire x1="-1.3649" y1="4.4851" x2="-0.9385" y2="4.4851" width="0.1016" layer="21"/>
<wire x1="-0.9385" y1="4.4851" x2="-0.801" y2="4.3476" width="0.1016" layer="21"/>
<wire x1="-0.801" y1="4.3476" x2="-0.801" y2="3.6557" width="0.1016" layer="21"/>
<wire x1="-0.801" y1="3.6557" x2="-0.7536" y2="3.5562" width="0.1016" layer="21" curve="50.916048"/>
<wire x1="-0.7536" y1="3.5562" x2="-0.5403" y2="3.4804" width="0.1016" layer="21" curve="39.080411"/>
<wire x1="-0.5403" y1="3.4804" x2="-0.5403" y2="4.4329" width="0.1016" layer="51"/>
<wire x1="-0.5403" y1="4.4329" x2="0.5117" y2="4.4329" width="0.1016" layer="51"/>
<wire x1="0.5117" y1="4.4329" x2="0.5117" y2="3.9496" width="0.1016" layer="51"/>
<wire x1="0.5117" y1="3.9496" x2="-0.019" y2="3.9496" width="0.1016" layer="51"/>
<wire x1="-0.019" y1="3.9496" x2="-0.019" y2="3.8027" width="0.1016" layer="51"/>
<wire x1="-0.019" y1="3.8027" x2="0.6302" y2="3.8027" width="0.1016" layer="51"/>
<wire x1="0.6302" y1="3.8027" x2="0.6302" y2="3.4567" width="0.1016" layer="51"/>
<wire x1="0.6302" y1="3.4567" x2="0.8577" y2="3.4567" width="0.1016" layer="51"/>
<wire x1="0.8577" y1="3.4567" x2="0.8577" y2="3.9401" width="0.1016" layer="51"/>
<wire x1="0.8577" y1="3.9401" x2="1.5591" y2="3.9401" width="0.1016" layer="51"/>
<wire x1="1.5591" y1="3.9401" x2="1.5591" y2="2.2293" width="0.1016" layer="21"/>
<wire x1="1.538" y1="2.2293" x2="1.2131" y2="2.2293" width="0.1016" layer="21"/>
<wire x1="1.2131" y1="-2.2293" x2="1.2131" y2="2.2293" width="0.1016" layer="21"/>
<wire x1="-1.4407" y1="3.6226" x2="-1.0047" y2="3.3667" width="0.1016" layer="25"/>
<wire x1="-1.0047" y1="3.3667" x2="-1.4407" y2="3.0823" width="0.1016" layer="25"/>
<wire x1="-2.995" y1="-4.35" x2="-2.8649" y2="-4.4851" width="0.1016" layer="21"/>
<wire x1="-2.8649" y1="-4.4851" x2="-2.4385" y2="-4.4851" width="0.1016" layer="21" style="shortdash"/>
<wire x1="-2.995" y1="-4.35" x2="-2.995" y2="4.35" width="0.1016" layer="21" style="shortdash"/>
<wire x1="-2.995" y1="4.35" x2="-2.8649" y2="4.4851" width="0.1016" layer="21"/>
<wire x1="-2.8649" y1="4.4851" x2="-2.4385" y2="4.4851" width="0.1016" layer="21" style="shortdash"/>
<wire x1="-2.4385" y1="4.4851" x2="-2.301" y2="4.3476" width="0.1016" layer="21"/>
<wire x1="-2.4385" y1="-4.4851" x2="-2.301" y2="-4.3476" width="0.1016" layer="21"/>
<wire x1="-2.301" y1="-4.3476" x2="-2.301" y2="-3.6557" width="0.1016" layer="21" style="shortdash"/>
<wire x1="-2.301" y1="4.3476" x2="-2.301" y2="3.6557" width="0.1016" layer="21" style="shortdash"/>
<wire x1="-2.2536" y1="3.5562" x2="-2.0403" y2="3.4804" width="0.1016" layer="21" curve="39.080411"/>
<wire x1="-2.2536" y1="-3.5562" x2="-2.0403" y2="-3.4804" width="0.1016" layer="21" curve="-39.080411"/>
<wire x1="-2.301" y1="-3.6557" x2="-2.2536" y2="-3.5562" width="0.1016" layer="21" curve="-50.916048"/>
<wire x1="-2.301" y1="3.6557" x2="-2.2536" y2="3.5562" width="0.1016" layer="21" curve="50.916048"/>
<circle x="-1.3412" y="3.3667" radius="0.0948" width="0" layer="25"/>
<smd name="1" x="1.905" y="1.75" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="2" x="1.905" y="1.25" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="3" x="1.905" y="0.75" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="4" x="1.905" y="0.25" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="5" x="1.905" y="-0.25" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="6" x="1.905" y="-0.75" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="7" x="1.905" y="-1.25" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="8" x="1.905" y="-1.75" dx="1" dy="0.25" layer="1" stop="no"/>
<smd name="M1" x="0.18" y="4.1375" dx="1.34" dy="1" layer="1"/>
<smd name="M2" x="0.18" y="-4.1375" dx="1.34" dy="1" layer="1"/>
<text x="-1.5" y="5" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.5" y="-6.5" size="1.27" layer="27">&gt;VALUE</text>
<text x="-0.5" y="3" size="0.4064" layer="51" font="vector" rot="SR270">Lower-contact</text>
<rectangle x1="1.355" y1="1.575" x2="2.455" y2="1.925" layer="29"/>
<rectangle x1="1.355" y1="1.075" x2="2.455" y2="1.425" layer="29"/>
<rectangle x1="1.355" y1="0.575" x2="2.455" y2="0.925" layer="29"/>
<rectangle x1="1.355" y1="0.075" x2="2.455" y2="0.425" layer="29"/>
<rectangle x1="1.355" y1="-0.425" x2="2.455" y2="-0.075" layer="29"/>
<rectangle x1="1.355" y1="-0.925" x2="2.455" y2="-0.575" layer="29"/>
<rectangle x1="1.355" y1="-1.425" x2="2.455" y2="-1.075" layer="29"/>
<rectangle x1="1.355" y1="-1.925" x2="2.455" y2="-1.575" layer="29"/>
<rectangle x1="0.105" y1="-4.45" x2="0.805" y2="-3.925" layer="51"/>
<rectangle x1="-0.445" y1="-4.45" x2="0.105" y2="-3.65" layer="51"/>
<rectangle x1="0.105" y1="-4.45" x2="0.805" y2="-3.925" layer="51"/>
<rectangle x1="-0.445" y1="-4.45" x2="0.105" y2="-3.65" layer="51"/>
<rectangle x1="0.105" y1="3.925" x2="0.805" y2="4.45" layer="51"/>
<rectangle x1="-0.445" y1="3.65" x2="0.105" y2="4.45" layer="51"/>
<rectangle x1="1.255" y1="1.65" x2="1.98" y2="1.85" layer="51"/>
<rectangle x1="1.255" y1="1.15" x2="1.98" y2="1.35" layer="51"/>
<rectangle x1="1.255" y1="0.65" x2="1.98" y2="0.85" layer="51"/>
<rectangle x1="1.255" y1="0.15" x2="1.98" y2="0.35" layer="51"/>
<rectangle x1="1.255" y1="-0.35" x2="1.98" y2="-0.15" layer="51"/>
<rectangle x1="1.255" y1="-0.85" x2="1.98" y2="-0.65" layer="51"/>
<rectangle x1="1.255" y1="-1.35" x2="1.98" y2="-1.15" layer="51"/>
<rectangle x1="1.255" y1="-1.85" x2="1.98" y2="-1.65" layer="51"/>
</package>
</packages>
<symbols>
<symbol name="PV">
<wire x1="0" y1="0" x2="1.27" y2="0" width="0.6096" layer="94"/>
<text x="0" y="1.27" size="1.778" layer="96">&gt;VALUE</text>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<pin name="MSV" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
<symbol name="P">
<wire x1="0" y1="0" x2="1.27" y2="0" width="0.6096" layer="94"/>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<pin name="MS" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="XF2*-08?" prefix="J">
<description>&lt;b&gt;OMRON FPC Connector&lt;/b&gt;&lt;p&gt;
Source : http://www.omron.com/ecb/products/pdf/fpc.pdf</description>
<gates>
<gate name="-1" symbol="PV" x="0" y="0" addlevel="always" swaplevel="1"/>
<gate name="-2" symbol="P" x="0" y="-2.54" addlevel="always" swaplevel="1"/>
<gate name="-3" symbol="P" x="0" y="-5.08" addlevel="always" swaplevel="1"/>
<gate name="-4" symbol="P" x="0" y="-7.62" addlevel="always" swaplevel="1"/>
<gate name="-5" symbol="P" x="0" y="-10.16" addlevel="always" swaplevel="1"/>
<gate name="-6" symbol="P" x="0" y="-12.7" addlevel="always" swaplevel="1"/>
<gate name="-7" symbol="P" x="0" y="-15.24" addlevel="always" swaplevel="1"/>
<gate name="-8" symbol="P" x="0" y="-17.78" addlevel="always" swaplevel="1"/>
</gates>
<devices>
<device name="15-1" package="XF2E-0815-1">
<connects>
<connect gate="-1" pin="MSV" pad="1"/>
<connect gate="-2" pin="MS" pad="2"/>
<connect gate="-3" pin="MS" pad="3"/>
<connect gate="-4" pin="MS" pad="4"/>
<connect gate="-5" pin="MS" pad="5"/>
<connect gate="-6" pin="MS" pad="6"/>
<connect gate="-7" pin="MS" pad="7"/>
<connect gate="-8" pin="MS" pad="8"/>
</connects>
<technologies>
<technology name="E"/>
</technologies>
</device>
<device name="24-11" package="XF2J-0824-11">
<connects>
<connect gate="-1" pin="MSV" pad="1"/>
<connect gate="-2" pin="MS" pad="2"/>
<connect gate="-3" pin="MS" pad="3"/>
<connect gate="-4" pin="MS" pad="4"/>
<connect gate="-5" pin="MS" pad="5"/>
<connect gate="-6" pin="MS" pad="6"/>
<connect gate="-7" pin="MS" pad="7"/>
<connect gate="-8" pin="MS" pad="8"/>
</connects>
<technologies>
<technology name="J"/>
</technologies>
</device>
<device name="24-12" package="XF2J-0824-12">
<connects>
<connect gate="-1" pin="MSV" pad="1"/>
<connect gate="-2" pin="MS" pad="2"/>
<connect gate="-3" pin="MS" pad="3"/>
<connect gate="-4" pin="MS" pad="4"/>
<connect gate="-5" pin="MS" pad="5"/>
<connect gate="-6" pin="MS" pad="6"/>
<connect gate="-7" pin="MS" pad="7"/>
<connect gate="-8" pin="MS" pad="8"/>
</connects>
<technologies>
<technology name="J"/>
</technologies>
</device>
<device name="25-1" package="XF2L-0825-1">
<connects>
<connect gate="-1" pin="MSV" pad="1"/>
<connect gate="-2" pin="MS" pad="2"/>
<connect gate="-3" pin="MS" pad="3"/>
<connect gate="-4" pin="MS" pad="4"/>
<connect gate="-5" pin="MS" pad="5"/>
<connect gate="-6" pin="MS" pad="6"/>
<connect gate="-7" pin="MS" pad="7"/>
<connect gate="-8" pin="MS" pad="8"/>
</connects>
<technologies>
<technology name="L"/>
</technologies>
</device>
<device name="35-1" package="XF2L-0835-1">
<connects>
<connect gate="-1" pin="MSV" pad="1"/>
<connect gate="-2" pin="MS" pad="2"/>
<connect gate="-3" pin="MS" pad="3"/>
<connect gate="-4" pin="MS" pad="4"/>
<connect gate="-5" pin="MS" pad="5"/>
<connect gate="-6" pin="MS" pad="6"/>
<connect gate="-7" pin="MS" pad="7"/>
<connect gate="-8" pin="MS" pad="8"/>
</connects>
<technologies>
<technology name="L"/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="rectifier">
<description>&lt;b&gt;Rectifiers&lt;/b&gt;&lt;p&gt;
General Instrument, Semikron, Diotec, Fagor&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="DSI32A">
<description>&lt;b&gt;RECTIFIER&lt;/b&gt;&lt;p&gt;
3.2 A</description>
<wire x1="-9.6712" y1="0.865" x2="-8.6552" y2="0.865" width="0.1524" layer="21" curve="180"/>
<wire x1="-8.6552" y1="0.865" x2="-7.6392" y2="0.865" width="0.1524" layer="21" curve="-180"/>
<wire x1="6.5848" y1="0.865" x2="7.6008" y2="0.865" width="0.1524" layer="21" curve="-180"/>
<wire x1="5.5688" y1="0.865" x2="6.5848" y2="0.865" width="0.1524" layer="21" curve="180"/>
<wire x1="13.316" y1="0.865" x2="14.586" y2="0.865" width="0.1524" layer="21"/>
<wire x1="-1.924" y1="0.865" x2="-0.654" y2="0.865" width="0.1524" layer="21"/>
<wire x1="-1.289" y1="0.23" x2="-1.289" y2="1.5" width="0.1524" layer="21"/>
<wire x1="10.725" y1="3.278" x2="10.725" y2="-0.405" width="0.1524" layer="51"/>
<wire x1="10.725" y1="-0.405" x2="10.725" y2="-3.326" width="0.1524" layer="21"/>
<wire x1="10.725" y1="-3.326" x2="16.059" y2="-3.326" width="0.1524" layer="21"/>
<wire x1="16.059" y1="-3.326" x2="16.059" y2="3.278" width="0.1524" layer="21"/>
<wire x1="-20.009" y1="3.278" x2="-20.009" y2="-3.326" width="0.1524" layer="21"/>
<wire x1="-20.009" y1="3.278" x2="16.059" y2="3.278" width="0.1524" layer="21"/>
<wire x1="-20.009" y1="-3.326" x2="10.725" y2="-3.326" width="0.1524" layer="21"/>
<pad name="AC1" x="-11.5" y="1.5" drill="1.3208" shape="long" rot="R90"/>
<pad name="+" x="-3.9816" y="1.5" drill="1.3208" shape="long" rot="R90"/>
<pad name="AC2" x="3.5114" y="1.5" drill="1.3208" shape="long" rot="R90"/>
<pad name="-" x="11.0044" y="1.5" drill="1.3208" shape="long" rot="R90"/>
<text x="-20.2122" y="3.786" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-11.5762" y="3.786" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-20" y1="-3.5" x2="16" y2="3.5" layer="43"/>
</package>
</packages>
<symbols>
<symbol name="GB">
<wire x1="0" y1="-2.54" x2="2.54" y2="0" width="0.3048" layer="94"/>
<wire x1="0" y1="2.54" x2="-2.54" y2="0" width="0.3048" layer="94"/>
<wire x1="-2.54" y1="0" x2="0" y2="-2.54" width="0.3048" layer="94"/>
<wire x1="2.54" y1="0" x2="0" y2="2.54" width="0.3048" layer="94"/>
<wire x1="-1.27" y1="0" x2="0.635" y2="0" width="0.1524" layer="94"/>
<wire x1="-0.635" y1="0.635" x2="-0.635" y2="-0.635" width="0.1524" layer="94"/>
<wire x1="-0.635" y1="0.635" x2="0.635" y2="0" width="0.1524" layer="94"/>
<wire x1="0.635" y1="0" x2="1.27" y2="0" width="0.1524" layer="94"/>
<wire x1="0.635" y1="0" x2="-0.635" y2="-0.635" width="0.1524" layer="94"/>
<wire x1="0.635" y1="0.635" x2="0.635" y2="-0.635" width="0.1524" layer="94"/>
<text x="2.54" y="2.54" size="1.778" layer="95">&gt;NAME</text>
<text x="2.54" y="-3.81" size="1.778" layer="96">&gt;VALUE</text>
<pin name="AC1" x="0" y="5.08" visible="pad" length="short" direction="pas" rot="R270"/>
<pin name="AC2" x="0" y="-5.08" visible="pad" length="short" direction="pas" rot="R90"/>
<pin name="-" x="-5.08" y="0" visible="pad" length="short" direction="pas"/>
<pin name="+" x="5.08" y="0" visible="pad" length="short" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="DSI32A" prefix="B" uservalue="yes">
<description>&lt;b&gt;RECTIFIER&lt;/b&gt;&lt;p&gt;
3.2 A</description>
<gates>
<gate name="1" symbol="GB" x="0" y="0"/>
</gates>
<devices>
<device name="" package="DSI32A">
<connects>
<connect gate="1" pin="+" pad="+"/>
<connect gate="1" pin="-" pad="-"/>
<connect gate="1" pin="AC1" pad="AC1"/>
<connect gate="1" pin="AC2" pad="AC2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="fuse">
<description>&lt;b&gt;Fuses and Fuse Holders&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="SH25">
<description>&lt;b&gt;FUSE HOLDER&lt;/b&gt;&lt;p&gt; grid 2,5 mm,  OGN0031 8211 Schurter (Buerklin)</description>
<wire x1="-13.081" y1="-5.461" x2="13.081" y2="-5.461" width="0.1524" layer="21"/>
<wire x1="13.081" y1="5.461" x2="-13.081" y2="5.461" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="-2.921" x2="5.08" y2="-2.921" width="0.0508" layer="21"/>
<wire x1="-5.08" y1="2.921" x2="5.08" y2="2.921" width="0.0508" layer="21"/>
<wire x1="-10.795" y1="3.683" x2="-5.715" y2="3.683" width="0.1524" layer="21"/>
<wire x1="-10.795" y1="-3.683" x2="-10.795" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="-10.795" y1="-2.54" x2="-5.715" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-10.795" y1="3.683" x2="-10.795" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-10.795" y1="2.54" x2="-5.715" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="-3.683" x2="-10.795" y2="-3.683" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="-3.683" x2="-5.715" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-10.795" y1="-3.683" x2="-11.43" y2="-3.683" width="0.1524" layer="21"/>
<wire x1="-11.43" y1="-3.683" x2="-11.43" y2="-2.032" width="0.1524" layer="21"/>
<wire x1="-11.176" y1="-2.032" x2="-11.176" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="-11.176" y1="-3.175" x2="-10.795" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="-10.795" y1="-3.175" x2="-10.795" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="2.54" x2="-5.715" y2="3.683" width="0.1524" layer="21"/>
<wire x1="-11.43" y1="-2.032" x2="-11.43" y2="-1.27" width="0.1524" layer="51"/>
<wire x1="-11.43" y1="-1.27" x2="-11.176" y2="-1.27" width="0.1524" layer="51"/>
<wire x1="-11.176" y1="-1.27" x2="-11.176" y2="-2.032" width="0.1524" layer="51"/>
<wire x1="10.795" y1="-3.683" x2="5.715" y2="-3.683" width="0.1524" layer="21"/>
<wire x1="10.795" y1="3.683" x2="10.795" y2="3.175" width="0.1524" layer="21"/>
<wire x1="10.795" y1="2.54" x2="5.715" y2="2.54" width="0.1524" layer="21"/>
<wire x1="10.795" y1="-3.683" x2="10.795" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="10.795" y1="-2.54" x2="5.715" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="5.715" y1="3.683" x2="10.795" y2="3.683" width="0.1524" layer="21"/>
<wire x1="5.715" y1="3.683" x2="5.715" y2="2.54" width="0.1524" layer="21"/>
<wire x1="10.795" y1="3.683" x2="11.43" y2="3.683" width="0.1524" layer="21"/>
<wire x1="11.43" y1="3.683" x2="11.43" y2="2.032" width="0.1524" layer="21"/>
<wire x1="11.176" y1="2.032" x2="11.176" y2="3.175" width="0.1524" layer="21"/>
<wire x1="11.176" y1="3.175" x2="10.795" y2="3.175" width="0.1524" layer="21"/>
<wire x1="10.795" y1="3.175" x2="10.795" y2="2.54" width="0.1524" layer="21"/>
<wire x1="5.715" y1="-2.54" x2="5.715" y2="-3.683" width="0.1524" layer="21"/>
<wire x1="11.43" y1="2.032" x2="11.43" y2="1.27" width="0.1524" layer="51"/>
<wire x1="11.43" y1="1.27" x2="11.176" y2="1.27" width="0.1524" layer="51"/>
<wire x1="11.176" y1="1.27" x2="11.176" y2="2.032" width="0.1524" layer="51"/>
<wire x1="-5.207" y1="0" x2="5.207" y2="0" width="0.0508" layer="21"/>
<wire x1="13.081" y1="5.461" x2="13.081" y2="2.032" width="0.1524" layer="21"/>
<wire x1="13.081" y1="-2.032" x2="13.081" y2="2.032" width="0.1524" layer="51"/>
<wire x1="13.081" y1="-2.032" x2="13.081" y2="-5.461" width="0.1524" layer="21"/>
<wire x1="-13.081" y1="5.461" x2="-13.081" y2="2.032" width="0.1524" layer="21"/>
<wire x1="-13.081" y1="-2.032" x2="-13.081" y2="2.032" width="0.1524" layer="51"/>
<wire x1="-13.081" y1="-2.032" x2="-13.081" y2="-5.461" width="0.1524" layer="21"/>
<pad name="1" x="-12.5222" y="0" drill="1.3208" diameter="2.54" shape="long" rot="R90"/>
<pad name="2" x="12.5222" y="0" drill="1.3208" diameter="2.54" shape="long" rot="R90"/>
<text x="-12.7" y="-7.62" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="0" y="-7.62" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-10.541" y1="-2.032" x2="-10.16" y2="2.032" layer="21"/>
<rectangle x1="-10.541" y1="1.905" x2="-9.652" y2="2.032" layer="21"/>
<rectangle x1="-10.541" y1="-2.032" x2="-9.652" y2="-1.905" layer="21"/>
<rectangle x1="-5.715" y1="-3.175" x2="-5.08" y2="-2.032" layer="21"/>
<rectangle x1="-5.715" y1="2.032" x2="-5.08" y2="3.175" layer="21"/>
<rectangle x1="-5.715" y1="-2.032" x2="-5.08" y2="2.032" layer="21"/>
<rectangle x1="10.16" y1="-2.032" x2="10.541" y2="2.032" layer="21"/>
<rectangle x1="9.652" y1="-2.032" x2="10.541" y2="-1.905" layer="21"/>
<rectangle x1="9.652" y1="1.905" x2="10.541" y2="2.032" layer="21"/>
<rectangle x1="5.08" y1="2.032" x2="5.715" y2="3.175" layer="21"/>
<rectangle x1="5.08" y1="-3.175" x2="5.715" y2="-2.032" layer="21"/>
<rectangle x1="5.08" y1="-2.032" x2="5.715" y2="2.032" layer="21"/>
<rectangle x1="-5.08" y1="2.159" x2="5.08" y2="2.667" layer="21"/>
<rectangle x1="-5.08" y1="-2.667" x2="5.08" y2="-2.159" layer="21"/>
<rectangle x1="5.715" y1="-2.032" x2="9.652" y2="-1.905" layer="21"/>
<rectangle x1="5.715" y1="1.905" x2="9.652" y2="2.032" layer="21"/>
<rectangle x1="-9.652" y1="1.905" x2="-5.715" y2="2.032" layer="21"/>
<rectangle x1="-9.652" y1="-2.032" x2="-5.715" y2="-1.905" layer="21"/>
<rectangle x1="-10.541" y1="-2.54" x2="-5.715" y2="-2.032" layer="21"/>
<rectangle x1="-10.541" y1="2.032" x2="-5.715" y2="2.54" layer="21"/>
<rectangle x1="5.715" y1="2.032" x2="10.541" y2="2.54" layer="21"/>
<rectangle x1="5.715" y1="-2.54" x2="10.541" y2="-2.032" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="FUSE">
<wire x1="-3.81" y1="-0.762" x2="3.81" y2="-0.762" width="0.254" layer="94"/>
<wire x1="3.81" y1="0.762" x2="-3.81" y2="0.762" width="0.254" layer="94"/>
<wire x1="3.81" y1="-0.762" x2="3.81" y2="0.762" width="0.254" layer="94"/>
<wire x1="-3.81" y1="0.762" x2="-3.81" y2="-0.762" width="0.254" layer="94"/>
<wire x1="-2.54" y1="0" x2="2.54" y2="0" width="0.1524" layer="94"/>
<text x="-3.81" y="1.397" size="1.778" layer="95">&gt;NAME</text>
<text x="-3.81" y="-2.921" size="1.778" layer="96">&gt;VALUE</text>
<pin name="2" x="5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1" rot="R180"/>
<pin name="1" x="-5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="SH25" prefix="F" uservalue="yes">
<description>&lt;b&gt;FUSE HOLDER&lt;/b&gt;&lt;p&gt; grid 25 mm, OGN0031 8211, Schurter (Buerklin)</description>
<gates>
<gate name="1" symbol="FUSE" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SH25">
<connects>
<connect gate="1" pin="1" pad="1"/>
<connect gate="1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply2">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
Please keep in mind, that these devices are necessary for the
automatic wiring of the supply signals.&lt;p&gt;
The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="GND">
<wire x1="-1.27" y1="0" x2="1.27" y2="0" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="0" y2="-1.27" width="0.254" layer="94"/>
<wire x1="0" y1="-1.27" x2="-1.27" y2="0" width="0.254" layer="94"/>
<text x="-1.905" y="-3.175" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
<symbol name="+12V">
<wire x1="0" y1="1.905" x2="0" y2="0.635" width="0.1524" layer="94"/>
<wire x1="-0.635" y1="1.27" x2="0.635" y2="1.27" width="0.1524" layer="94"/>
<circle x="0" y="1.27" radius="1.27" width="0.254" layer="94"/>
<text x="-2.54" y="3.175" size="1.778" layer="96">&gt;VALUE</text>
<pin name="+12V" x="0" y="-2.54" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
<symbol name="-12V">
<wire x1="-0.635" y1="-1.27" x2="0.635" y2="-1.27" width="0.1524" layer="94"/>
<circle x="0" y="-1.27" radius="1.27" width="0.254" layer="94"/>
<text x="-3.175" y="-4.699" size="1.778" layer="96">&gt;VALUE</text>
<pin name="-12V" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="GND" prefix="SUPPLY">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="GND" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="+12V" prefix="SUPPLY">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="+12V" symbol="+12V" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="-12V" prefix="SUPPLY">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="-12V" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="TR1" library="trafo" deviceset="EE20-2" device=""/>
<part name="J1" library="con-omron" deviceset="XF2*-08?" device="15-1" technology="E"/>
<part name="B1" library="rectifier" deviceset="DSI32A" device=""/>
<part name="F1" library="fuse" deviceset="SH25" device=""/>
<part name="F2" library="fuse" deviceset="SH25" device=""/>
<part name="F3" library="fuse" deviceset="SH25" device=""/>
<part name="SUPPLY1" library="supply2" deviceset="GND" device=""/>
<part name="SUPPLY2" library="supply2" deviceset="GND" device=""/>
<part name="SUPPLY4" library="supply2" deviceset="+12V" device=""/>
<part name="SUPPLY3" library="supply2" deviceset="-12V" device=""/>
<part name="SUPPLY5" library="supply2" deviceset="-12V" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<text x="-175.26" y="104.14" size="1.778" layer="91">220V 50Hz AC</text>
</plain>
<instances>
<instance part="TR1" gate="1" x="-88.9" y="91.44"/>
<instance part="J1" gate="-1" x="-142.24" y="81.28" rot="R180"/>
<instance part="J1" gate="-2" x="-142.24" y="83.82" rot="R180"/>
<instance part="J1" gate="-3" x="-142.24" y="86.36" rot="R180"/>
<instance part="J1" gate="-4" x="-142.24" y="88.9" rot="R180"/>
<instance part="J1" gate="-5" x="-142.24" y="91.44" rot="R180"/>
<instance part="J1" gate="-6" x="-142.24" y="93.98" rot="R180"/>
<instance part="J1" gate="-7" x="-142.24" y="96.52" rot="R180"/>
<instance part="J1" gate="-8" x="-142.24" y="99.06" rot="R180"/>
<instance part="B1" gate="1" x="-58.42" y="88.9"/>
<instance part="F1" gate="1" x="-172.72" y="99.06"/>
<instance part="F2" gate="1" x="-157.48" y="93.98"/>
<instance part="F3" gate="1" x="-167.64" y="88.9"/>
<instance part="SUPPLY1" gate="GND" x="-78.74" y="88.9"/>
<instance part="SUPPLY2" gate="GND" x="-78.74" y="88.9"/>
<instance part="SUPPLY4" gate="+12V" x="-106.68" y="86.36"/>
<instance part="SUPPLY3" gate="G$1" x="-124.46" y="83.82"/>
<instance part="SUPPLY5" gate="G$1" x="-124.46" y="83.82"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<pinref part="J1" gate="-8" pin="MS"/>
<pinref part="TR1" gate="1" pin="1"/>
<wire x1="-139.7" y1="99.06" x2="-93.98" y2="99.06" width="0.1524" layer="91"/>
<wire x1="-93.98" y1="99.06" x2="-93.98" y2="96.52" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="J1" gate="-7" pin="MS"/>
<wire x1="-139.7" y1="96.52" x2="-96.52" y2="96.52" width="0.1524" layer="91"/>
<wire x1="-96.52" y1="96.52" x2="-96.52" y2="83.82" width="0.1524" layer="91"/>
<pinref part="TR1" gate="1" pin="2"/>
<wire x1="-96.52" y1="83.82" x2="-93.98" y2="83.82" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="TR1" gate="1" pin="6"/>
<wire x1="-81.28" y1="96.52" x2="-58.42" y2="96.52" width="0.1524" layer="91"/>
<pinref part="B1" gate="1" pin="AC1"/>
<wire x1="-58.42" y1="96.52" x2="-58.42" y2="93.98" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="TR1" gate="1" pin="3"/>
<pinref part="B1" gate="1" pin="AC2"/>
<wire x1="-81.28" y1="83.82" x2="-58.42" y2="83.82" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="B1" gate="1" pin="+"/>
<wire x1="-53.34" y1="88.9" x2="-53.34" y2="71.12" width="0.1524" layer="91"/>
<wire x1="-53.34" y1="71.12" x2="-116.84" y2="71.12" width="0.1524" layer="91"/>
<wire x1="-116.84" y1="71.12" x2="-116.84" y2="88.9" width="0.1524" layer="91"/>
<pinref part="J1" gate="-4" pin="MS"/>
<wire x1="-116.84" y1="88.9" x2="-139.7" y2="88.9" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="B1" gate="1" pin="-"/>
<wire x1="-63.5" y1="88.9" x2="-63.5" y2="86.36" width="0.1524" layer="91"/>
<wire x1="-63.5" y1="86.36" x2="-63.5" y2="81.28" width="0.1524" layer="91" curve="-180"/>
<wire x1="-63.5" y1="81.28" x2="-63.5" y2="73.66" width="0.1524" layer="91"/>
<wire x1="-63.5" y1="73.66" x2="-99.06" y2="73.66" width="0.1524" layer="91"/>
<wire x1="-99.06" y1="73.66" x2="-99.06" y2="93.98" width="0.1524" layer="91"/>
<pinref part="J1" gate="-6" pin="MS"/>
<wire x1="-99.06" y1="93.98" x2="-139.7" y2="93.98" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="F1" gate="1" pin="2"/>
<wire x1="-167.64" y1="99.06" x2="-142.24" y2="99.06" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<wire x1="-142.24" y1="96.52" x2="-180.34" y2="96.52" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="F2" gate="1" pin="2"/>
<wire x1="-142.24" y1="93.98" x2="-152.4" y2="93.98" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="F2" gate="1" pin="1"/>
<wire x1="-162.56" y1="93.98" x2="-162.56" y2="91.44" width="0.1524" layer="91"/>
<wire x1="-162.56" y1="91.44" x2="-142.24" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<pinref part="F3" gate="1" pin="2"/>
<wire x1="-142.24" y1="88.9" x2="-162.56" y2="88.9" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<pinref part="F3" gate="1" pin="1"/>
<wire x1="-172.72" y1="88.9" x2="-172.72" y2="86.36" width="0.1524" layer="91"/>
<wire x1="-172.72" y1="86.36" x2="-142.24" y2="86.36" width="0.1524" layer="91"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<pinref part="SUPPLY1" gate="GND" pin="GND"/>
<pinref part="SUPPLY2" gate="GND" pin="GND"/>
<pinref part="TR1" gate="1" pin="5"/>
<pinref part="TR1" gate="1" pin="4"/>
<wire x1="-81.28" y1="91.44" x2="-81.28" y2="88.9" width="0.1524" layer="91"/>
<wire x1="-81.28" y1="91.44" x2="-78.74" y2="91.44" width="0.1524" layer="91"/>
</segment>
</net>
<net name="+12V" class="0">
<segment>
<pinref part="J1" gate="-5" pin="MS"/>
<wire x1="-139.7" y1="91.44" x2="-111.76" y2="91.44" width="0.1524" layer="91"/>
<wire x1="-111.76" y1="91.44" x2="-111.76" y2="83.82" width="0.1524" layer="91"/>
<pinref part="SUPPLY4" gate="+12V" pin="+12V"/>
<wire x1="-111.76" y1="83.82" x2="-106.68" y2="83.82" width="0.1524" layer="91"/>
</segment>
</net>
<net name="-12V" class="0">
<segment>
<pinref part="SUPPLY3" gate="G$1" pin="-12V"/>
<pinref part="SUPPLY5" gate="G$1" pin="-12V"/>
<pinref part="J1" gate="-3" pin="MS"/>
<wire x1="-139.7" y1="86.36" x2="-124.46" y2="86.36" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="F1" gate="1" pin="1"/>
<wire x1="-177.8" y1="99.06" x2="-180.34" y2="99.06" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
