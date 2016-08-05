// to be inlined by the render manager for now

#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <stdlib.h>
#include <vector>

namespace
{


const float gridVertices[] = {
	-0.852243, 0.852243, 0.000000, 0.000000, 1.000000,
	-0.788650, 0.876276, 0.000000, 0.050000, 1.000000,
	-0.719374, 0.899219, 0.000000, 0.100000, 1.000000,
	-0.644480, 0.920687, 0.000000, 0.150000, 1.000000,
	-0.564177, 0.940297, 0.000000, 0.200000, 1.000000,
	-0.478834, 0.957664, 0.000000, 0.250000, 1.000000,
	-0.388972, 0.972431, 0.000000, 0.300000, 1.000000,
	-0.295285, 0.984283, 0.000000, 0.350000, 1.000000,
	-0.198589, 0.992945, 0.000000, 0.400000, 1.000000,
	-0.099823, 0.998228, 0.000000, 0.450000, 1.000000,
	0.000000, 1.000000, 0.000000, 0.500000, 1.000000,
	0.099823, 0.998228, 0.000000, 0.550000, 1.000000,
	0.198589, 0.992945, 0.000000, 0.600000, 1.000000,
	0.295285, 0.984283, 0.000000, 0.650000, 1.000000,
	0.388972, 0.972431, 0.000000, 0.700000, 1.000000,
	0.478834, 0.957664, 0.000000, 0.750000, 1.000000,
	0.564177, 0.940297, 0.000000, 0.800000, 1.000000,
	0.644480, 0.920687, 0.000000, 0.850000, 1.000000,
	0.719374, 0.899219, 0.000000, 0.900000, 1.000000,
	0.788650, 0.876276, 0.000000, 0.950000, 1.000000,
	0.852243, 0.852243, 0.000000, 1.000000, 1.000000,
	-0.876276, 0.788650, 0.000000, 0.000000, 0.950000,
	-0.811811, 0.811811, 0.000000, 0.050000, 0.950000,
	-0.741296, 0.833958, 0.000000, 0.100000, 0.950000,
	-0.664776, 0.854711, 0.000000, 0.150000, 0.950000,
	-0.582458, 0.873689, 0.000000, 0.200000, 0.950000,
	-0.494729, 0.890514, 0.000000, 0.250000, 0.950000,
	-0.402146, 0.904830, 0.000000, 0.300000, 0.950000,
	-0.305440, 0.916318, 0.000000, 0.350000, 0.950000,
	-0.205494, 0.924723, 0.000000, 0.400000, 0.950000,
	-0.103316, 0.929845, 0.000000, 0.450000, 0.950000,
	0.000000, 0.931567, 0.000000, 0.500000, 0.950000,
	0.103316, 0.929845, 0.000000, 0.550000, 0.950000,
	0.205494, 0.924723, 0.000000, 0.600000, 0.950000,
	0.305440, 0.916318, 0.000000, 0.650000, 0.950000,
	0.402146, 0.904830, 0.000000, 0.700000, 0.950000,
	0.494729, 0.890514, 0.000000, 0.750000, 0.950000,
	0.582458, 0.873689, 0.000000, 0.800000, 0.950000,
	0.664776, 0.854711, 0.000000, 0.850000, 0.950000,
	0.741296, 0.833958, 0.000000, 0.900000, 0.950000,
	0.811811, 0.811811, 0.000000, 0.950000, 0.950000,
	0.876276, 0.788650, 0.000000, 1.000000, 0.950000,
	-0.899219, 0.719374, 0.000000, 0.000000, 0.900000,
	-0.833958, 0.741296, 0.000000, 0.050000, 0.900000,
	-0.762285, 0.762285, 0.000000, 0.100000, 0.900000,
	-0.684229, 0.781978, 0.000000, 0.150000, 0.900000,
	-0.600001, 0.800002, 0.000000, 0.200000, 0.900000,
	-0.509993, 0.815991, 0.000000, 0.250000, 0.900000,
	-0.414798, 0.829596, 0.000000, 0.300000, 0.900000,
	-0.315195, 0.840520, 0.000000, 0.350000, 0.900000,
	-0.212127, 0.848507, 0.000000, 0.400000, 0.900000,
	-0.106672, 0.853375, 0.000000, 0.450000, 0.900000,
	0.000000, 0.855011, 0.000000, 0.500000, 0.900000,
	0.106672, 0.853375, 0.000000, 0.550000, 0.900000,
	0.212127, 0.848507, 0.000000, 0.600000, 0.900000,
	0.315195, 0.840520, 0.000000, 0.650000, 0.900000,
	0.414798, 0.829596, 0.000000, 0.700000, 0.900000,
	0.509993, 0.815991, 0.000000, 0.750000, 0.900000,
	0.600001, 0.800002, 0.000000, 0.800000, 0.900000,
	0.684229, 0.781978, 0.000000, 0.850000, 0.900000,
	0.762285, 0.762285, 0.000000, 0.900000, 0.900000,
	0.833958, 0.741296, 0.000000, 0.950000, 0.900000,
	0.899219, 0.719374, 0.000000, 1.000000, 0.900000,
	-0.920687, 0.644480, 0.000000, 0.000000, 0.850000,
	-0.854711, 0.664776, 0.000000, 0.050000, 0.850000,
	-0.781978, 0.684229, 0.000000, 0.100000, 0.850000,
	-0.702500, 0.702500, 0.000000, 0.150000, 0.850000,
	-0.616482, 0.719231, 0.000000, 0.200000, 0.850000,
	-0.524338, 0.734073, 0.000000, 0.250000, 0.850000,
	-0.426689, 0.746704, 0.000000, 0.300000, 0.850000,
	-0.324359, 0.756838, 0.000000, 0.350000, 0.850000,
	-0.218357, 0.764250, 0.000000, 0.400000, 0.850000,
	-0.109823, 0.768761, 0.000000, 0.450000, 0.850000,
	0.000000, 0.770276, 0.000000, 0.500000, 0.850000,
	0.109823, 0.768761, 0.000000, 0.550000, 0.850000,
	0.218357, 0.764250, 0.000000, 0.600000, 0.850000,
	0.324359, 0.756838, 0.000000, 0.650000, 0.850000,
	0.426689, 0.746704, 0.000000, 0.700000, 0.850000,
	0.524338, 0.734073, 0.000000, 0.750000, 0.850000,
	0.616482, 0.719231, 0.000000, 0.800000, 0.850000,
	0.702500, 0.702500, 0.000000, 0.850000, 0.850000,
	0.781978, 0.684229, 0.000000, 0.900000, 0.850000,
	0.854711, 0.664776, 0.000000, 0.950000, 0.850000,
	0.920687, 0.644480, 0.000000, 1.000000, 0.850000,
	-0.940297, 0.564177, 0.000000, 0.000000, 0.800000,
	-0.873689, 0.582458, 0.000000, 0.050000, 0.800000,
	-0.800002, 0.600001, 0.000000, 0.100000, 0.800000,
	-0.719231, 0.616482, 0.000000, 0.150000, 0.800000,
	-0.631577, 0.631577, 0.000000, 0.200000, 0.800000,
	-0.537472, 0.644965, 0.000000, 0.250000, 0.800000,
	-0.437570, 0.656353, 0.000000, 0.300000, 0.800000,
	-0.332742, 0.665483, 0.000000, 0.350000, 0.800000,
	-0.224050, 0.672152, 0.000000, 0.400000, 0.800000,
	-0.112701, 0.676210, 0.000000, 0.450000, 0.800000,
	0.000000, 0.677571, 0.000000, 0.500000, 0.800000,
	0.112701, 0.676210, 0.000000, 0.550000, 0.800000,
	0.224050, 0.672152, 0.000000, 0.600000, 0.800000,
	0.332742, 0.665483, 0.000000, 0.650000, 0.800000,
	0.437570, 0.656353, 0.000000, 0.700000, 0.800000,
	0.537472, 0.644965, 0.000000, 0.750000, 0.800000,
	0.631577, 0.631577, 0.000000, 0.800000, 0.800000,
	0.719231, 0.616482, 0.000000, 0.850000, 0.800000,
	0.800002, 0.600001, 0.000000, 0.900000, 0.800000,
	0.873689, 0.582458, 0.000000, 0.950000, 0.800000,
	0.940297, 0.564177, 0.000000, 1.000000, 0.800000,
	-0.957664, 0.478834, 0.000000, 0.000000, 0.750000,
	-0.890514, 0.494729, 0.000000, 0.050000, 0.750000,
	-0.815991, 0.509993, 0.000000, 0.100000, 0.750000,
	-0.734073, 0.524338, 0.000000, 0.150000, 0.750000,
	-0.644965, 0.537472, 0.000000, 0.200000, 0.750000,
	-0.549114, 0.549114, 0.000000, 0.250000, 0.750000,
	-0.447204, 0.559008, 0.000000, 0.300000, 0.750000,
	-0.340155, 0.566924, 0.000000, 0.350000, 0.750000,
	-0.229080, 0.572700, 0.000000, 0.400000, 0.750000,
	-0.115242, 0.576211, 0.000000, 0.450000, 0.750000,
	0.000000, 0.577390, 0.000000, 0.500000, 0.750000,
	0.115242, 0.576211, 0.000000, 0.550000, 0.750000,
	0.229080, 0.572700, 0.000000, 0.600000, 0.750000,
	0.340155, 0.566924, 0.000000, 0.650000, 0.750000,
	0.447204, 0.559008, 0.000000, 0.700000, 0.750000,
	0.549114, 0.549114, 0.000000, 0.750000, 0.750000,
	0.644965, 0.537472, 0.000000, 0.800000, 0.750000,
	0.734073, 0.524338, 0.000000, 0.850000, 0.750000,
	0.815991, 0.509993, 0.000000, 0.900000, 0.750000,
	0.890514, 0.494729, 0.000000, 0.950000, 0.750000,
	0.957664, 0.478834, 0.000000, 1.000000, 0.750000,
	-0.972431, 0.388972, 0.000000, 0.000000, 0.700000,
	-0.904830, 0.402146, 0.000000, 0.050000, 0.700000,
	-0.829596, 0.414798, 0.000000, 0.100000, 0.700000,
	-0.746704, 0.426689, 0.000000, 0.150000, 0.700000,
	-0.656353, 0.437570, 0.000000, 0.200000, 0.700000,
	-0.559008, 0.447204, 0.000000, 0.250000, 0.700000,
	-0.455380, 0.455380, 0.000000, 0.300000, 0.700000,
	-0.346433, 0.461910, 0.000000, 0.350000, 0.700000,
	-0.233332, 0.466664, 0.000000, 0.400000, 0.700000,
	-0.117388, 0.469550, 0.000000, 0.450000, 0.700000,
	0.000000, 0.470518, 0.000000, 0.500000, 0.700000,
	0.117388, 0.469550, 0.000000, 0.550000, 0.700000,
	0.233332, 0.466664, 0.000000, 0.600000, 0.700000,
	0.346433, 0.461910, 0.000000, 0.650000, 0.700000,
	0.455380, 0.455380, 0.000000, 0.700000, 0.700000,
	0.559008, 0.447204, 0.000000, 0.750000, 0.700000,
	0.656353, 0.437570, 0.000000, 0.800000, 0.700000,
	0.746704, 0.426689, 0.000000, 0.850000, 0.700000,
	0.829596, 0.414798, 0.000000, 0.900000, 0.700000,
	0.904830, 0.402146, 0.000000, 0.950000, 0.700000,
	0.972431, 0.388972, 0.000000, 1.000000, 0.700000,
	-0.984283, 0.295285, 0.000000, 0.000000, 0.650000,
	-0.916318, 0.305440, 0.000000, 0.050000, 0.650000,
	-0.840520, 0.315195, 0.000000, 0.100000, 0.650000,
	-0.756838, 0.324359, 0.000000, 0.150000, 0.650000,
	-0.665483, 0.332742, 0.000000, 0.200000, 0.650000,
	-0.566924, 0.340155, 0.000000, 0.250000, 0.650000,
	-0.461910, 0.346433, 0.000000, 0.300000, 0.650000,
	-0.351439, 0.351439, 0.000000, 0.350000, 0.650000,
	-0.236717, 0.355075, 0.000000, 0.400000, 0.650000,
	-0.119092, 0.357278, 0.000000, 0.450000, 0.650000,
	0.000000, 0.358014, 0.000000, 0.500000, 0.650000,
	0.119092, 0.357278, 0.000000, 0.550000, 0.650000,
	0.236717, 0.355075, 0.000000, 0.600000, 0.650000,
	0.351439, 0.351439, 0.000000, 0.650000, 0.650000,
	0.461910, 0.346433, 0.000000, 0.700000, 0.650000,
	0.566924, 0.340155, 0.000000, 0.750000, 0.650000,
	0.665483, 0.332742, 0.000000, 0.800000, 0.650000,
	0.756838, 0.324359, 0.000000, 0.850000, 0.650000,
	0.840520, 0.315195, 0.000000, 0.900000, 0.650000,
	0.916318, 0.305440, 0.000000, 0.950000, 0.650000,
	0.984283, 0.295285, 0.000000, 1.000000, 0.650000,
	-0.992945, 0.198589, 0.000000, 0.000000, 0.600000,
	-0.924723, 0.205494, 0.000000, 0.050000, 0.600000,
	-0.848507, 0.212127, 0.000000, 0.100000, 0.600000,
	-0.764250, 0.218357, 0.000000, 0.150000, 0.600000,
	-0.672152, 0.224050, 0.000000, 0.200000, 0.600000,
	-0.572700, 0.229080, 0.000000, 0.250000, 0.600000,
	-0.466664, 0.233332, 0.000000, 0.300000, 0.600000,
	-0.355075, 0.236717, 0.000000, 0.350000, 0.600000,
	-0.239169, 0.239169, 0.000000, 0.400000, 0.600000,
	-0.120326, 0.240653, 0.000000, 0.450000, 0.600000,
	0.000000, 0.241149, 0.000000, 0.500000, 0.600000,
	0.120326, 0.240653, 0.000000, 0.550000, 0.600000,
	0.239169, 0.239169, 0.000000, 0.600000, 0.600000,
	0.355075, 0.236717, 0.000000, 0.650000, 0.600000,
	0.466664, 0.233332, 0.000000, 0.700000, 0.600000,
	0.572700, 0.229080, 0.000000, 0.750000, 0.600000,
	0.672152, 0.224050, 0.000000, 0.800000, 0.600000,
	0.764250, 0.218357, 0.000000, 0.850000, 0.600000,
	0.848507, 0.212127, 0.000000, 0.900000, 0.600000,
	0.924723, 0.205494, 0.000000, 0.950000, 0.600000,
	0.992945, 0.198589, 0.000000, 1.000000, 0.600000,
	-0.998228, 0.099823, 0.000000, 0.000000, 0.550000,
	-0.929845, 0.103316, 0.000000, 0.050000, 0.550000,
	-0.853375, 0.106672, 0.000000, 0.100000, 0.550000,
	-0.768761, 0.109823, 0.000000, 0.150000, 0.550000,
	-0.676210, 0.112701, 0.000000, 0.200000, 0.550000,
	-0.576211, 0.115242, 0.000000, 0.250000, 0.550000,
	-0.469550, 0.117388, 0.000000, 0.300000, 0.550000,
	-0.357278, 0.119092, 0.000000, 0.350000, 0.550000,
	-0.240653, 0.120326, 0.000000, 0.400000, 0.550000,
	-0.121071, 0.121071, 0.000000, 0.450000, 0.550000,
	0.000000, 0.121320, 0.000000, 0.500000, 0.550000,
	0.121071, 0.121071, 0.000000, 0.550000, 0.550000,
	0.240653, 0.120326, 0.000000, 0.600000, 0.550000,
	0.357278, 0.119092, 0.000000, 0.650000, 0.550000,
	0.469550, 0.117388, 0.000000, 0.700000, 0.550000,
	0.576211, 0.115242, 0.000000, 0.750000, 0.550000,
	0.676210, 0.112701, 0.000000, 0.800000, 0.550000,
	0.768761, 0.109823, 0.000000, 0.850000, 0.550000,
	0.853375, 0.106672, 0.000000, 0.900000, 0.550000,
	0.929845, 0.103316, 0.000000, 0.950000, 0.550000,
	0.998228, 0.099823, 0.000000, 1.000000, 0.550000,
	-1.000000, -0.000000, 0.000000, 0.000000, 0.500000,
	-0.931567, -0.000000, 0.000000, 0.050000, 0.500000,
	-0.855011, -0.000000, 0.000000, 0.100000, 0.500000,
	-0.770276, -0.000000, 0.000000, 0.150000, 0.500000,
	-0.677571, -0.000000, 0.000000, 0.200000, 0.500000,
	-0.577390, -0.000000, 0.000000, 0.250000, 0.500000,
	-0.470518, -0.000000, 0.000000, 0.300000, 0.500000,
	-0.358014, -0.000000, 0.000000, 0.350000, 0.500000,
	-0.241149, -0.000000, 0.000000, 0.400000, 0.500000,
	-0.121320, -0.000000, 0.000000, 0.450000, 0.500000,
	0.000000, -0.000000, 0.000000, 0.500000, 0.500000,
	0.121320, -0.000000, 0.000000, 0.550000, 0.500000,
	0.241149, -0.000000, 0.000000, 0.600000, 0.500000,
	0.358014, -0.000000, 0.000000, 0.650000, 0.500000,
	0.470518, -0.000000, 0.000000, 0.700000, 0.500000,
	0.577390, -0.000000, 0.000000, 0.750000, 0.500000,
	0.677571, -0.000000, 0.000000, 0.800000, 0.500000,
	0.770276, -0.000000, 0.000000, 0.850000, 0.500000,
	0.855011, -0.000000, 0.000000, 0.900000, 0.500000,
	0.931567, -0.000000, 0.000000, 0.950000, 0.500000,
	1.000000, -0.000000, 0.000000, 1.000000, 0.500000,
	-0.998228, -0.099823, 0.000000, 0.000000, 0.450000,
	-0.929845, -0.103316, 0.000000, 0.050000, 0.450000,
	-0.853375, -0.106672, 0.000000, 0.100000, 0.450000,
	-0.768761, -0.109823, 0.000000, 0.150000, 0.450000,
	-0.676210, -0.112701, 0.000000, 0.200000, 0.450000,
	-0.576211, -0.115242, 0.000000, 0.250000, 0.450000,
	-0.469550, -0.117388, 0.000000, 0.300000, 0.450000,
	-0.357278, -0.119092, 0.000000, 0.350000, 0.450000,
	-0.240653, -0.120326, 0.000000, 0.400000, 0.450000,
	-0.121071, -0.121071, 0.000000, 0.450000, 0.450000,
	0.000000, -0.121320, 0.000000, 0.500000, 0.450000,
	0.121071, -0.121071, 0.000000, 0.550000, 0.450000,
	0.240653, -0.120326, 0.000000, 0.600000, 0.450000,
	0.357278, -0.119092, 0.000000, 0.650000, 0.450000,
	0.469550, -0.117388, 0.000000, 0.700000, 0.450000,
	0.576211, -0.115242, 0.000000, 0.750000, 0.450000,
	0.676210, -0.112701, 0.000000, 0.800000, 0.450000,
	0.768761, -0.109823, 0.000000, 0.850000, 0.450000,
	0.853375, -0.106672, 0.000000, 0.900000, 0.450000,
	0.929845, -0.103316, 0.000000, 0.950000, 0.450000,
	0.998228, -0.099823, 0.000000, 1.000000, 0.450000,
	-0.992945, -0.198589, 0.000000, 0.000000, 0.400000,
	-0.924723, -0.205494, 0.000000, 0.050000, 0.400000,
	-0.848507, -0.212127, 0.000000, 0.100000, 0.400000,
	-0.764250, -0.218357, 0.000000, 0.150000, 0.400000,
	-0.672152, -0.224050, 0.000000, 0.200000, 0.400000,
	-0.572700, -0.229080, 0.000000, 0.250000, 0.400000,
	-0.466664, -0.233332, 0.000000, 0.300000, 0.400000,
	-0.355075, -0.236717, 0.000000, 0.350000, 0.400000,
	-0.239169, -0.239169, 0.000000, 0.400000, 0.400000,
	-0.120326, -0.240653, 0.000000, 0.450000, 0.400000,
	0.000000, -0.241149, 0.000000, 0.500000, 0.400000,
	0.120326, -0.240653, 0.000000, 0.550000, 0.400000,
	0.239169, -0.239169, 0.000000, 0.600000, 0.400000,
	0.355075, -0.236717, 0.000000, 0.650000, 0.400000,
	0.466664, -0.233332, 0.000000, 0.700000, 0.400000,
	0.572700, -0.229080, 0.000000, 0.750000, 0.400000,
	0.672152, -0.224050, 0.000000, 0.800000, 0.400000,
	0.764250, -0.218357, 0.000000, 0.850000, 0.400000,
	0.848507, -0.212127, 0.000000, 0.900000, 0.400000,
	0.924723, -0.205494, 0.000000, 0.950000, 0.400000,
	0.992945, -0.198589, 0.000000, 1.000000, 0.400000,
	-0.984283, -0.295285, 0.000000, 0.000000, 0.350000,
	-0.916318, -0.305440, 0.000000, 0.050000, 0.350000,
	-0.840520, -0.315195, 0.000000, 0.100000, 0.350000,
	-0.756838, -0.324359, 0.000000, 0.150000, 0.350000,
	-0.665483, -0.332742, 0.000000, 0.200000, 0.350000,
	-0.566924, -0.340155, 0.000000, 0.250000, 0.350000,
	-0.461910, -0.346433, 0.000000, 0.300000, 0.350000,
	-0.351439, -0.351439, 0.000000, 0.350000, 0.350000,
	-0.236717, -0.355075, 0.000000, 0.400000, 0.350000,
	-0.119092, -0.357278, 0.000000, 0.450000, 0.350000,
	0.000000, -0.358014, 0.000000, 0.500000, 0.350000,
	0.119092, -0.357278, 0.000000, 0.550000, 0.350000,
	0.236717, -0.355075, 0.000000, 0.600000, 0.350000,
	0.351439, -0.351439, 0.000000, 0.650000, 0.350000,
	0.461910, -0.346433, 0.000000, 0.700000, 0.350000,
	0.566924, -0.340155, 0.000000, 0.750000, 0.350000,
	0.665483, -0.332742, 0.000000, 0.800000, 0.350000,
	0.756838, -0.324359, 0.000000, 0.850000, 0.350000,
	0.840520, -0.315195, 0.000000, 0.900000, 0.350000,
	0.916318, -0.305440, 0.000000, 0.950000, 0.350000,
	0.984283, -0.295285, 0.000000, 1.000000, 0.350000,
	-0.972431, -0.388972, 0.000000, 0.000000, 0.300000,
	-0.904830, -0.402146, 0.000000, 0.050000, 0.300000,
	-0.829596, -0.414798, 0.000000, 0.100000, 0.300000,
	-0.746704, -0.426689, 0.000000, 0.150000, 0.300000,
	-0.656353, -0.437570, 0.000000, 0.200000, 0.300000,
	-0.559008, -0.447204, 0.000000, 0.250000, 0.300000,
	-0.455380, -0.455380, 0.000000, 0.300000, 0.300000,
	-0.346433, -0.461910, 0.000000, 0.350000, 0.300000,
	-0.233332, -0.466664, 0.000000, 0.400000, 0.300000,
	-0.117388, -0.469550, 0.000000, 0.450000, 0.300000,
	0.000000, -0.470518, 0.000000, 0.500000, 0.300000,
	0.117388, -0.469550, 0.000000, 0.550000, 0.300000,
	0.233332, -0.466664, 0.000000, 0.600000, 0.300000,
	0.346433, -0.461910, 0.000000, 0.650000, 0.300000,
	0.455380, -0.455380, 0.000000, 0.700000, 0.300000,
	0.559008, -0.447204, 0.000000, 0.750000, 0.300000,
	0.656353, -0.437570, 0.000000, 0.800000, 0.300000,
	0.746704, -0.426689, 0.000000, 0.850000, 0.300000,
	0.829596, -0.414798, 0.000000, 0.900000, 0.300000,
	0.904830, -0.402146, 0.000000, 0.950000, 0.300000,
	0.972431, -0.388972, 0.000000, 1.000000, 0.300000,
	-0.957664, -0.478834, 0.000000, 0.000000, 0.250000,
	-0.890514, -0.494729, 0.000000, 0.050000, 0.250000,
	-0.815991, -0.509993, 0.000000, 0.100000, 0.250000,
	-0.734073, -0.524338, 0.000000, 0.150000, 0.250000,
	-0.644965, -0.537472, 0.000000, 0.200000, 0.250000,
	-0.549114, -0.549114, 0.000000, 0.250000, 0.250000,
	-0.447204, -0.559008, 0.000000, 0.300000, 0.250000,
	-0.340155, -0.566924, 0.000000, 0.350000, 0.250000,
	-0.229080, -0.572700, 0.000000, 0.400000, 0.250000,
	-0.115242, -0.576211, 0.000000, 0.450000, 0.250000,
	0.000000, -0.577390, 0.000000, 0.500000, 0.250000,
	0.115242, -0.576211, 0.000000, 0.550000, 0.250000,
	0.229080, -0.572700, 0.000000, 0.600000, 0.250000,
	0.340155, -0.566924, 0.000000, 0.650000, 0.250000,
	0.447204, -0.559008, 0.000000, 0.700000, 0.250000,
	0.549114, -0.549114, 0.000000, 0.750000, 0.250000,
	0.644965, -0.537472, 0.000000, 0.800000, 0.250000,
	0.734073, -0.524338, 0.000000, 0.850000, 0.250000,
	0.815991, -0.509993, 0.000000, 0.900000, 0.250000,
	0.890514, -0.494729, 0.000000, 0.950000, 0.250000,
	0.957664, -0.478834, 0.000000, 1.000000, 0.250000,
	-0.940297, -0.564177, 0.000000, 0.000000, 0.200000,
	-0.873689, -0.582458, 0.000000, 0.050000, 0.200000,
	-0.800002, -0.600001, 0.000000, 0.100000, 0.200000,
	-0.719231, -0.616482, 0.000000, 0.150000, 0.200000,
	-0.631577, -0.631577, 0.000000, 0.200000, 0.200000,
	-0.537472, -0.644965, 0.000000, 0.250000, 0.200000,
	-0.437570, -0.656353, 0.000000, 0.300000, 0.200000,
	-0.332742, -0.665483, 0.000000, 0.350000, 0.200000,
	-0.224050, -0.672152, 0.000000, 0.400000, 0.200000,
	-0.112701, -0.676210, 0.000000, 0.450000, 0.200000,
	0.000000, -0.677571, 0.000000, 0.500000, 0.200000,
	0.112701, -0.676210, 0.000000, 0.550000, 0.200000,
	0.224050, -0.672152, 0.000000, 0.600000, 0.200000,
	0.332742, -0.665483, 0.000000, 0.650000, 0.200000,
	0.437570, -0.656353, 0.000000, 0.700000, 0.200000,
	0.537472, -0.644965, 0.000000, 0.750000, 0.200000,
	0.631577, -0.631577, 0.000000, 0.800000, 0.200000,
	0.719231, -0.616482, 0.000000, 0.850000, 0.200000,
	0.800002, -0.600001, 0.000000, 0.900000, 0.200000,
	0.873689, -0.582458, 0.000000, 0.950000, 0.200000,
	0.940297, -0.564177, 0.000000, 1.000000, 0.200000,
	-0.920687, -0.644480, 0.000000, 0.000000, 0.150000,
	-0.854711, -0.664776, 0.000000, 0.050000, 0.150000,
	-0.781978, -0.684229, 0.000000, 0.100000, 0.150000,
	-0.702500, -0.702500, 0.000000, 0.150000, 0.150000,
	-0.616482, -0.719231, 0.000000, 0.200000, 0.150000,
	-0.524338, -0.734073, 0.000000, 0.250000, 0.150000,
	-0.426689, -0.746704, 0.000000, 0.300000, 0.150000,
	-0.324359, -0.756838, 0.000000, 0.350000, 0.150000,
	-0.218357, -0.764250, 0.000000, 0.400000, 0.150000,
	-0.109823, -0.768761, 0.000000, 0.450000, 0.150000,
	0.000000, -0.770276, 0.000000, 0.500000, 0.150000,
	0.109823, -0.768761, 0.000000, 0.550000, 0.150000,
	0.218357, -0.764250, 0.000000, 0.600000, 0.150000,
	0.324359, -0.756838, 0.000000, 0.650000, 0.150000,
	0.426689, -0.746704, 0.000000, 0.700000, 0.150000,
	0.524338, -0.734073, 0.000000, 0.750000, 0.150000,
	0.616482, -0.719231, 0.000000, 0.800000, 0.150000,
	0.702500, -0.702500, 0.000000, 0.850000, 0.150000,
	0.781978, -0.684229, 0.000000, 0.900000, 0.150000,
	0.854711, -0.664776, 0.000000, 0.950000, 0.150000,
	0.920687, -0.644480, 0.000000, 1.000000, 0.150000,
	-0.899219, -0.719374, 0.000000, 0.000000, 0.100000,
	-0.833958, -0.741296, 0.000000, 0.050000, 0.100000,
	-0.762285, -0.762285, 0.000000, 0.100000, 0.100000,
	-0.684229, -0.781978, 0.000000, 0.150000, 0.100000,
	-0.600001, -0.800002, 0.000000, 0.200000, 0.100000,
	-0.509993, -0.815991, 0.000000, 0.250000, 0.100000,
	-0.414798, -0.829596, 0.000000, 0.300000, 0.100000,
	-0.315195, -0.840520, 0.000000, 0.350000, 0.100000,
	-0.212127, -0.848507, 0.000000, 0.400000, 0.100000,
	-0.106672, -0.853375, 0.000000, 0.450000, 0.100000,
	0.000000, -0.855011, 0.000000, 0.500000, 0.100000,
	0.106672, -0.853375, 0.000000, 0.550000, 0.100000,
	0.212127, -0.848507, 0.000000, 0.600000, 0.100000,
	0.315195, -0.840520, 0.000000, 0.650000, 0.100000,
	0.414798, -0.829596, 0.000000, 0.700000, 0.100000,
	0.509993, -0.815991, 0.000000, 0.750000, 0.100000,
	0.600001, -0.800002, 0.000000, 0.800000, 0.100000,
	0.684229, -0.781978, 0.000000, 0.850000, 0.100000,
	0.762285, -0.762285, 0.000000, 0.900000, 0.100000,
	0.833958, -0.741296, 0.000000, 0.950000, 0.100000,
	0.899219, -0.719374, 0.000000, 1.000000, 0.100000,
	-0.876276, -0.788650, 0.000000, 0.000000, 0.050000,
	-0.811811, -0.811811, 0.000000, 0.050000, 0.050000,
	-0.741296, -0.833958, 0.000000, 0.100000, 0.050000,
	-0.664776, -0.854711, 0.000000, 0.150000, 0.050000,
	-0.582458, -0.873689, 0.000000, 0.200000, 0.050000,
	-0.494729, -0.890514, 0.000000, 0.250000, 0.050000,
	-0.402146, -0.904830, 0.000000, 0.300000, 0.050000,
	-0.305440, -0.916318, 0.000000, 0.350000, 0.050000,
	-0.205494, -0.924723, 0.000000, 0.400000, 0.050000,
	-0.103316, -0.929845, 0.000000, 0.450000, 0.050000,
	0.000000, -0.931567, 0.000000, 0.500000, 0.050000,
	0.103316, -0.929845, 0.000000, 0.550000, 0.050000,
	0.205494, -0.924723, 0.000000, 0.600000, 0.050000,
	0.305440, -0.916318, 0.000000, 0.650000, 0.050000,
	0.402146, -0.904830, 0.000000, 0.700000, 0.050000,
	0.494729, -0.890514, 0.000000, 0.750000, 0.050000,
	0.582458, -0.873689, 0.000000, 0.800000, 0.050000,
	0.664776, -0.854711, 0.000000, 0.850000, 0.050000,
	0.741296, -0.833958, 0.000000, 0.900000, 0.050000,
	0.811811, -0.811811, 0.000000, 0.950000, 0.050000,
	0.876276, -0.788650, 0.000000, 1.000000, 0.050000,
	-0.852243, -0.852243, 0.000000, 0.000000, 0.000000,
	-0.788650, -0.876276, 0.000000, 0.050000, 0.000000,
	-0.719374, -0.899219, 0.000000, 0.100000, 0.000000,
	-0.644480, -0.920687, 0.000000, 0.150000, 0.000000,
	-0.564177, -0.940297, 0.000000, 0.200000, 0.000000,
	-0.478834, -0.957664, 0.000000, 0.250000, 0.000000,
	-0.388972, -0.972431, 0.000000, 0.300000, 0.000000,
	-0.295285, -0.984283, 0.000000, 0.350000, 0.000000,
	-0.198589, -0.992945, 0.000000, 0.400000, 0.000000,
	-0.099823, -0.998228, 0.000000, 0.450000, 0.000000,
	0.000000, -1.000000, 0.000000, 0.500000, 0.000000,
	0.099823, -0.998228, 0.000000, 0.550000, 0.000000,
	0.198589, -0.992945, 0.000000, 0.600000, 0.000000,
	0.295285, -0.984283, 0.000000, 0.650000, 0.000000,
	0.388972, -0.972431, 0.000000, 0.700000, 0.000000,
	0.478834, -0.957664, 0.000000, 0.750000, 0.000000,
	0.564177, -0.940297, 0.000000, 0.800000, 0.000000,
	0.644480, -0.920687, 0.000000, 0.850000, 0.000000,
	0.719374, -0.899219, 0.000000, 0.900000, 0.000000,
	0.788650, -0.876276, 0.000000, 0.950000, 0.000000,
	0.852243, -0.852243, 0.000000, 1.000000, 0.000000
};

inline void GenerateGridVertexBuffer( std::vector<float>& vertexBuffer )
{
  const int N = sizeof( gridVertices ) / sizeof( float );
  vertexBuffer.resize( N*2 );
  std::copy( gridVertices, gridVertices+N, vertexBuffer.data() );
  std::copy( gridVertices, gridVertices+N, vertexBuffer.data()+N );

  // process data for both eyes


  for( size_t i = 0; i < N; i += 5 )
  {
    float* co = &vertexBuffer[i];
    float* uv = &vertexBuffer[i+3];
    co[1] -= 1.0f;
    co[1] *= 0.5f;
    uv[1] *= 0.5f;
  }

  //for( size_t i = N; i < N*2; i += 5 )
  for( size_t i = N; i < N*2; i += 5 )
  {
    float* co = &vertexBuffer[i];
    float* uv = &vertexBuffer[i+3];
    co[1] += 1.0f;
    co[1] *= 0.5f;
    uv[1] *= 0.5f;
    uv[1] += 1.0f;
  }

  return;
}

inline void GenerateGridIndexBuffer( std::vector<uint16_t>& indexBuffer )
{
  const int BASE = sizeof( gridVertices ) / sizeof( float ) / 5;
  const int N = sqrt( BASE );
  indexBuffer.resize( N*N*6*2 );
  uint16_t* gridData = indexBuffer.data();//(uint16_t*)malloc(N*N*6*sizeof(uint16_t));
  int j, i, k = 0;

  // left
  for(j=0; j<N-1; j++)
  {
    for(i=0; i<N-1; i++)
    {
      gridData[k] = i + N*j;
      gridData[k+1] = (i+1) + j*N;
      gridData[k+2] = i + (j+1)*N;
      gridData[k+3] = (i+1) + j*N;
      gridData[k+4] = (i+1) + (j+1)*N;
      gridData[k+5] = i + (j+1)*N;
      k += 6;
    }
  }

  // right
  for(j=0; j<N-1; j++)
  {
    for(i=0; i<N-1; i++)
    {
      gridData[k] = BASE + (i + N*j);
      gridData[k+1] = BASE +( (i+1) + j*N );
      gridData[k+2] = BASE +( i + (j+1)*N );
      gridData[k+3] = BASE + ( (i+1) + j*N );
      gridData[k+4] = BASE + ( (i+1) + (j+1)*N );
      gridData[k+5] = BASE + (i + (j+1)*N);
      k += 6;
    }
  }
  return;
}

}

