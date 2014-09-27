#ifndef CONNECTIONGENE_HPP
#define CONNECTIONGENE_HPP

namespace NEAT_USM{
	class connectionGene{
		public:
			
			/**
			* \brief Change or set the value to the variables to innovation, in, out, weight, enable.
			* \param innovation the innovation of the genne_connection.
			* \param in is the input node.
			* \param out is the output node.
			* \param weight is the weight of the connection.
			* \param enable if this connection is present in the genome.
			*
			*/
			connectionGene(
				int innovation,
				int in, 
				int out,
				double weight,
				bool enable
				); // fill a conection gene

			/**
				\brief Get connection innovation number. 
			*/
			int 	getInnovation();
			/**
				\brief Get node input of connection. 
			*/
			int 	getIn();
			/**
				\brief Get node output of connection. 
			*/
			int 	getOut();
			/**
				\brief Get true if connection is enable or false in other case.
			*/
			bool	getEnable();
			/**
				\brief Get synaptic weight of connection. 
			*/
			double 	getWeight();
			/**
				\brief Set connection innovation number.  WARNING: is not recomended to use.
				\param _innovation is the innovation number. 
			*/
			void setInnovation(int _innovation);
			/**
				\brief Get connection innovation number.
				\param _in is the input node.
			*/
			void setIn(int _in);
			/**
				\brief Get connection innovation number.
				\param _out is the output node. 
			*/
			void setOut(int _out);
			/**
				\brief Get connection innovation number.
				\param _enable is the enable state. 
			*/
			void setEnable(bool _enable);
			/**
				\brief Get connection innovation number 
				\param _weight is the synaptic weight.
			*/
			void setWeight(double _weight);


		private:
			int 	innovation;
			int 	in;
			int 	out;
			bool	enable;
			double 	weight;
	};
} 
#endif