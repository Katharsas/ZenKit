// Copyright © 2021 Luis Michaelis
// Licensed under MIT (https://mit-license.org/).
#include <phoenix/softskin_mesh.hh>

#include <fmt/format.h>

namespace phoenix {
	enum class softmesh_chunk { unknown, header = 0xE100, end = 0xE110, proto = 0xB100, nodes = 0xB1FF };

	softskin_mesh softskin_mesh::parse(buffer& in) {
		softskin_mesh msh;
		softmesh_chunk type = softmesh_chunk::unknown;
		bool end_mesh = false;

		do {
			type = static_cast<softmesh_chunk>(in.get_ushort());

			auto length = in.get_uint();
			auto chunk = in.extract(length);

			switch (type) {
			case softmesh_chunk::header:
				(void) /* version = */ chunk.get_uint();
				break ;
			case softmesh_chunk::proto:
				msh._m_mesh = proto_mesh::parse_from_section(chunk);
				break;
			case softmesh_chunk::nodes: {
				// weights
				auto weight_buffer_size = chunk.get_uint();
				auto weight_buffer_end = chunk.position() + weight_buffer_size;

				msh._m_weights.reserve(chunk.get_uint());

				if (weight_buffer_size / (sizeof(float) * 4 + sizeof(std::uint8_t)) == msh._m_weights.size()) {
					fmt::print(stderr, "warning: softskin mesh: not all data from weight section\n");
				}

				for (std::uint32_t i = 0; i < msh._m_weights.size(); ++i) {
					auto& weight = msh._m_weights.emplace_back();
					weight.weight = chunk.get_float();
					weight.position = chunk.get_vec3();
					weight.node_index = chunk.get();
				}

				chunk.position(weight_buffer_end);

				// wedge normals
				msh._m_wedge_normals.reserve(chunk.get_uint());

				for (std::uint32_t i = 0; i < msh._m_wedge_normals.size(); ++i) {
					auto& normal = msh._m_wedge_normals.emplace_back();
					normal.normal = chunk.get_vec3();
					normal.index = chunk.get_uint();
				}

				// nodes
				msh._m_nodes.resize(chunk.get_ushort());

				for (std::uint32_t i = 0; i < msh._m_nodes.size(); ++i) {
					msh._m_nodes[i] = chunk.get_int();
				}

				for (std::uint32_t i = 0; i < msh._m_nodes.size(); ++i) {
					msh._m_bboxes.push_back(obb::parse(chunk));
				}

				break;
			}
			case softmesh_chunk::end:
				end_mesh = true;
				break;
			default:
				break;
			}

			if (chunk.remaining() != 0) {
				fmt::print(stderr, "warning: softskin mesh: not all data consumed from section 0x{:X}\n", type);
			}
		} while (!end_mesh);

		return msh;
	}
} // namespace phoenix
