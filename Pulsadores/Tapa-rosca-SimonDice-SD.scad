
$fn=100;

altura=30;

diametro_interior=27.5; //Antes 27.3
espesor=1.5;
diametro_agujero_cable=5;


rotate([0,180,0])
difference(){
	cylinder(r=diametro_interior/2+espesor, h=altura);

	cylinder(r=diametro_interior/2, h=altura-espesor);
	cylinder(r=diametro_agujero_cable/2, h=altura+espesor);
}