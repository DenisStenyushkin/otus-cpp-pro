#include <memory>

#include "Document.hpp"
#include "DocumentController.hpp"
#include "DocumentView.hpp"
#include "Line.hpp"
#include "Circle.hpp"
#include "CoordinatePoint.hpp"
#include "GraphPrimitive.hpp"


int main() {
    std::shared_ptr<Document> document = std::make_shared<Document>();
    std::shared_ptr<DocumentView> view = std::make_shared<DocumentView>();
    std::shared_ptr<DocumentController> controller = std::make_shared<DocumentController>(document, view);

    controller->AddElement(
        std::dynamic_pointer_cast<GraphPrimitive>(
            std::make_shared<Line>(CoordinatePoint{1, 2}, CoordinatePoint{3, 4})
        )
    );

    controller->AddElement(
        std::dynamic_pointer_cast<GraphPrimitive>(
            std::make_shared<Circle>(CoordinatePoint{10, 20}, 30)
        )
    );

    controller->MoveElement(0, CoordinatePoint{10, 20});
    controller->RemoveElement(1);
    controller->SaveDocument("document.doc");
    controller->LoadDocument("document.doc");
}
