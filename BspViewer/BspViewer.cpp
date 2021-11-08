#include "BspViewer.h"
#include <string>

BspViewer::BspViewer() {


}

BspViewer::BspViewer(std::string file_name) {
	m_file_name = file_name;
	open();

}

void BspViewer::open(std::string file_name) {
	m_file_name = file_name;
	open();

}

void BspViewer::open() {
	m_file_ptr.open(m_file_name);
	read();
}

void BspViewer::render() {

}

void BspViewer::render(std::string file_name) {
	open(file_name);
	render();
}

BspViewer::~BspViewer() {

}