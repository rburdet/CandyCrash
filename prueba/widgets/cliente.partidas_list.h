#ifndef __PARTIDAS_LIST_H__
#define __PARTIDAS_LIST_H__

#include <gtkmm.h>

class ModelColumns : public Gtk::TreeModel::ColumnRecord
{
	public:

		ModelColumns()
		{ add(m_col_nivel); add(m_col_id); add(m_col_nombre); add(m_col_conectados); add(m_col_percentage);}

		Gtk::TreeModelColumn<unsigned int> m_col_nivel;
		Gtk::TreeModelColumn<unsigned long> m_col_id;
		Gtk::TreeModelColumn<std::string> m_col_nombre;
		Gtk::TreeModelColumn<int> m_col_conectados;
		Gtk::TreeModelColumn<int> m_col_percentage;
};

class ListaPartidas : public Gtk::TreeView {
	public:
		ListaPartidas(){
			m_refTreeModel = Gtk::ListStore::create(m_Columns);
			this->set_model(m_refTreeModel);

			//This number will be shown with the default numeric formatting.
			this->append_column("Nivel", m_Columns.m_col_nivel);
			//this->append_column_numeric("Id", m_Columns.m_col_id, "%010d");  /* 10 digits, using leading zeroes. */
			this->append_column("Nombre", m_Columns.m_col_nombre);
			this->append_column("Conectados", m_Columns.m_col_conectados);

			//Display a progress bar instead of a decimal number:
			Gtk::CellRendererProgress* cell = Gtk::manage(new Gtk::CellRendererProgress);
			int cols_count = this->append_column("Lleno", *cell);
			Gtk::TreeViewColumn* pColumn = this->get_column(cols_count - 1);
			if(pColumn){
				pColumn->add_attribute(cell->property_value(), m_Columns.m_col_percentage);
			}

			//Make all the columns reorderable:
			//This is not necessary, but it's nice to show the feature.
			//You can use TreeView::set_column_drag_function() to more
			//finely control column drag and drop.
			for(guint i = 0; i < 3; i++)
			{
				Gtk::TreeView::Column* pColumn = this->get_column(i);
				pColumn->set_reorderable();
			}
		}
		~ListaPartidas(){};

		void addRow(unsigned int m_col_nivel, unsigned long m_col_id, std::string& m_col_nombre, short m_col_conectados, int m_col_percentage){
			Gtk::TreeModel::Row row = *(m_refTreeModel->append());
			row[m_Columns.m_col_nivel] = m_col_nivel;
			row[m_Columns.m_col_id] = m_col_id;
			row[m_Columns.m_col_nombre] = m_col_nombre;
			row[m_Columns.m_col_conectados] = m_col_conectados;
			row[m_Columns.m_col_percentage] = m_col_percentage;
		}

		void getSelected(unsigned int & nivel, long& id, std::string & nombre, int &conectados, int &percentage){
			Glib::RefPtr<Gtk::TreeSelection> select = this->get_selection();
			Gtk::TreeModel::iterator store_iter = select->get_selected();
			Gtk::TreeModel::Row row = *store_iter;
			nivel = row[m_Columns.m_col_nivel];
			id = row[m_Columns.m_col_id];
			nombre = row[m_Columns.m_col_nombre];
			conectados = row[m_Columns.m_col_conectados];
			percentage = row[m_Columns.m_col_percentage];

		}

		void clearRows(){
			m_refTreeModel->clear();
		}

	protected:
		ModelColumns m_Columns;
		Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

};

#endif
