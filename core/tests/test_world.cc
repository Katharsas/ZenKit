// Copyright © 2021 Luis Michaelis
// Licensed under MIT (https://mit-license.org/).
#include "catch2.hh"
#include <phoenix/world.hh>

TEST_CASE("world meshes are read correctly", "[world][mesh]") {
	auto in = phoenix::reader::from("./samples/world.proprietary.zen");
	auto wld = phoenix::world::read(in, game_version::gothic_1);

	auto& mesh = wld.mesh();
	REQUIRE(mesh.vertices().size() == 55439);
	REQUIRE(mesh.features().size() == 419936);
	REQUIRE(mesh.materials().size() == 2263);
	REQUIRE(mesh.name() == "");

	auto [min, max] = mesh.bbox();
	REQUIRE(min == glm::vec4 {0, 0, 0, 0});
	REQUIRE(max == glm::vec4 {0, 0, 0, 0});

	auto& verts = mesh.vertices();
	REQUIRE(verts[0] == glm::vec3 {91365, -4026.60083, 46900});
	REQUIRE(verts[1] == glm::vec3 {92900, -4029.99976, 38399.9961});
	REQUIRE(verts[500] == glm::vec3 {44263.8203, 708.517822, 6841.18262});
	REQUIRE(verts[501] == glm::vec3 {45672.6094, 640.436157, 6877.81543});

	auto& feats = mesh.features();
	REQUIRE(feats[0].texture == glm::vec2 {1.11193848, 2.64415169});
	REQUIRE(feats[0].light == 4292927712);
	REQUIRE(feats[0].normal == glm::vec3 {0.0000220107158, 1, -0.000121058853});

	REQUIRE(feats[1].texture == glm::vec2 {-0.371101379, -0.909111022});
	REQUIRE(feats[1].light == 4292927712);
	REQUIRE(feats[1].normal == glm::vec3 {0.0000251403726, 1, -0.000138271935});

	REQUIRE(feats[500].texture == glm::vec2 {-0.524971008, 2.59478664});
	REQUIRE(feats[500].light == 4281084972);
	REQUIRE(feats[500].normal == glm::vec3 {-0.000046945388, -0.99999994, 0.000258200336});

	REQUIRE(feats[501].texture == glm::vec2 {1.93376923, -0.734043121});
	REQUIRE(feats[501].light == 4281084972);
	REQUIRE(feats[501].normal == glm::vec3 {0.000102534526, -1, -0.00014051389});

	auto& mats = mesh.materials();
	auto& mat0 = mats[0];
	auto& mat500 = mats[500];

	REQUIRE(mat0.name() == "OWODWATSTOP");
	REQUIRE(mat0.group() == phoenix::material_group::water);
	REQUIRE(mat0.texture() == "OWODSEA_A0.TGA");

	REQUIRE(mat500.name() == "OMWABROWNGREEN01");
	REQUIRE(mat500.group() == phoenix::material_group::stone);
	REQUIRE(mat500.texture() == "OMWABROWNGREEN01.TGA");
}

TEST_CASE("the bsp-tree is read correctly", "[world][bsp]") {
	auto in = phoenix::reader::from("./samples/world.proprietary.zen");
	auto wld = phoenix::world::read(in, game_version::gothic_1);
	auto& tree = wld.tree();

	REQUIRE(tree.mode() == phoenix::bsp_tree_mode::outdoor);

	auto& polys = tree.polygon_indices();
	REQUIRE(polys.size() == 480135);
	REQUIRE(polys[0] == 0);
	REQUIRE(polys[1] == 1);
	REQUIRE(polys[2] == 2);
	REQUIRE(polys[150] == 102);
	REQUIRE(polys[151] == 103);
	REQUIRE(polys[152] == 92);
}

TEST_CASE("the vob-tree is read correctly", "[world][vob]") {
	auto in = phoenix::reader::from("./samples/world.proprietary.zen");
	auto wld = phoenix::world::read(in, game_version::gothic_1);
	auto& vobs = wld.vobs();

	REQUIRE(vobs.size() == 14);

	auto& vob0 = vobs[0];
	auto& vob13 = vobs[13];

	{
		auto [min, max] = vob0.bbox();
		REQUIRE(min == glm::vec3 {-71919.9609, -13091.8232, -59900});
		REQUIRE(max == glm::vec3 {108999.992, 20014.0352, 67399.9921});

		auto mat = vob0.rotation();
		REQUIRE(mat[0][0] == 1.0f);
		REQUIRE(mat[0][1] == 0.0f);
		REQUIRE(mat[0][2] == 0.0f);
		REQUIRE(mat[1][0] == 0.0f);
		REQUIRE(mat[1][1] == 1.0f);
		REQUIRE(mat[1][2] == 0.0f);
		REQUIRE(mat[2][0] == 0.0f);
		REQUIRE(mat[2][1] == 0.0f);
		REQUIRE(mat[2][2] == 1.0f);

		REQUIRE(vob0.vob_name() == "LEVEL-VOB");
		REQUIRE(vob0.visual_name() == "SURFACE.3DS");
		REQUIRE(vob0.preset_name().empty());
		REQUIRE(vob0.position() == glm::vec3 {0, 0, 0});
		REQUIRE(!vob0.show_visual());
		REQUIRE(vob0.camera_alignment() == 0);
		REQUIRE(vob0.animation_mode() == 0);
		REQUIRE(vob0.animation_strength() == 0.0f);
		REQUIRE(vob0.far_clip_scale() == 0);
		REQUIRE(vob0.is_cd_static());
		REQUIRE(!vob0.is_cd_dynamic());
		REQUIRE(!vob0.is_vob_static());
		REQUIRE(vob0.dynamic_shadows() == 0);
		REQUIRE(vob0.bias() == 0);
		REQUIRE(!vob0.is_ambient());
		REQUIRE(!vob0.is_physics_enabled());

		auto& children = vob0.children();
		REQUIRE(children.size() == 7496);

		auto& child1 = children[0];

		{
			auto [minc, maxc] = child1.bbox();
			REQUIRE(minc == glm::vec3 {-18596.9004, -161.17189, 4091.1333});
			REQUIRE(maxc == glm::vec3 {-18492.0723, -111.171906, 4191.26221});

			auto matc = child1.rotation();
			REQUIRE(matc[0][0] == -0.779196978f);
			REQUIRE(matc[0][1] == 0.0f);
			REQUIRE(matc[0][2] == 0.626779079f);
			REQUIRE(matc[1][0] == 0.0f);
			REQUIRE(matc[1][1] == 1.0f);
			REQUIRE(matc[1][2] == 0.0f);
			REQUIRE(matc[2][0] == -0.626779079f);
			REQUIRE(matc[2][1] == 0.0f);
			REQUIRE(matc[2][2] == -0.779196978f);

			REQUIRE(child1.vob_name() == "FP_CAMPFIRE_PATH_BANDITOS2_03_02");
			REQUIRE(child1.visual_name().empty());
			REQUIRE(child1.preset_name().empty());
			REQUIRE(child1.position() == glm::vec3 {-18544.4863, -136.171906, 4141.19727});
			REQUIRE(!child1.show_visual());
			REQUIRE(child1.camera_alignment() == 0);
			REQUIRE(child1.animation_mode() == 0);
			REQUIRE(child1.animation_strength() == 0.0f);
			REQUIRE(child1.far_clip_scale() == 0);
			REQUIRE(!child1.is_cd_static());
			REQUIRE(!child1.is_cd_dynamic());
			REQUIRE(!child1.is_vob_static());
			REQUIRE(child1.dynamic_shadows() == 0);
			REQUIRE(child1.bias() == 0);
			REQUIRE(!child1.is_ambient());
			REQUIRE(!child1.is_physics_enabled());

			REQUIRE(child1.children().empty());
		}
	}

	{
		auto [min, max] = vob13.bbox();
		REQUIRE(min == glm::vec3 {-9999.40234, -10000.0039, -9200});
		REQUIRE(max == glm::vec3 {9060.59765, 5909.90039, 7537.47461});

		auto mat = vob13.rotation();
		REQUIRE(mat[0][0] == 1.0f);
		REQUIRE(mat[0][1] == 0.0f);
		REQUIRE(mat[0][2] == 0.0f);
		REQUIRE(mat[1][0] == 0.0f);
		REQUIRE(mat[1][1] == 1.0f);
		REQUIRE(mat[1][2] == 0.0f);
		REQUIRE(mat[2][0] == 0.0f);
		REQUIRE(mat[2][1] == 0.0f);
		REQUIRE(mat[2][2] == 1.0f);

		REQUIRE(vob13.vob_name() == "LEVEL-VOB");
		REQUIRE(vob13.visual_name() == "OLDCAMP.3DS");
		REQUIRE(vob13.preset_name().empty());
		REQUIRE(vob13.position() == glm::vec3 {0, 0, 0});
		REQUIRE(!vob13.show_visual());
		REQUIRE(vob13.camera_alignment() == 0);
		REQUIRE(vob13.animation_mode() == 0);
		REQUIRE(vob13.animation_strength() == 0.0f);
		REQUIRE(vob13.far_clip_scale() == 0);
		REQUIRE(!vob13.is_cd_static());
		REQUIRE(!vob13.is_cd_dynamic());
		REQUIRE(!vob13.is_vob_static());
		REQUIRE(vob13.dynamic_shadows() == 0);
		REQUIRE(vob13.bias() == 0);
		REQUIRE(!vob13.is_ambient());
		REQUIRE(!vob13.is_physics_enabled());

		auto& children = vob13.children();
		REQUIRE(children.size() == 3250);
	}
}