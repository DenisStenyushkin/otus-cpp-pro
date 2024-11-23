#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include "CoordinatePoint.hpp"
#include "Document.hpp"
#include "DocumentView.hpp"
#include "GraphPrimitive.hpp"
#include "serialization.hpp"

class DocumentController {
public:
    DocumentController(std::shared_ptr<Document>& document, std::shared_ptr<DocumentView>& document_view)
        : m_document{document}, m_view{document_view} {};

    void AddElement(PrimitivePtr&& element) {
        m_document->AddElement(element);
        m_view->Update(m_document->GetDocumentPrimitives());
    }

    void MoveElement(std::size_t elementIdx, CoordinatePoint&& coord) {
        m_document->MoveElement(elementIdx, coord);
        m_view->Update(m_document->GetDocumentPrimitives());
    }

    void RemoveElement(std::size_t elementIdx) {
        m_document->RemoveElement(elementIdx);
        m_view->Update(m_document->GetDocumentPrimitives());
    }

    void SaveDocument(const std::string& file_name) {
        serialize(file_name, m_document->GetExportableState());
        m_view->Update("Saved successfully");
    }

    void LoadDocument(const std::string& file_name) {
        m_document = std::make_shared<Document>(deserialize(file_name));
        m_view->Update(m_document->GetDocumentPrimitives());
        m_view->Update("Loaded sussessfully");
    }
private:
    std::shared_ptr<Document> m_document;
    std::shared_ptr<DocumentView> m_view;
};