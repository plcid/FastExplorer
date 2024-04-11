#include "utils.hpp"

void utils::VerticalSeparator(int offset, int customheight) {
    ImVec2 scrpos = ImGui::GetCursorScreenPos();
    scrpos.x += offset;
    ImGui::GetWindowDrawList()->AddLine(scrpos, { scrpos.x, scrpos.y + (customheight ? customheight : ImGui::GetWindowSize().y) }, ImGui::GetColorU32(ImGuiCol_Separator));
    ImGui::SetCursorScreenPos(scrpos + ImVec2{ ImGui::GetStyle().ItemSpacing.x + 1,0 });
}