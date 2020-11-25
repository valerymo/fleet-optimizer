# fleet-optimizer

Fleet Optimizer it's an application that planned to be used, as part of Desision Support and Making System,
to help define the optimal structure of specific Marine Fleet - set of ships for performing required transportation tasks.

Fleet Optimizer is using Operation Research approaches and methods to find the optimal solution.  
Ships Routes creation done in Optimization time using Dynamic programming.  
Optimal Ships selected as variation of Ships load and speed.  
Characteristics of Ships calculated based on Ships and ACV Design methodologies. Please see references and notes. 

Please note, that Fleet Optimizer require tailoring before using for specific Fleet selection. Statistical analyses of specific conditions and specific vessels will be necessary.  
Possible application areas where Fleet Optimizer could be useful:

- Optimization of Fleet for service Marine Wind Farms
- Optimization of Service fleet for Oil and Gas Offshore platforms
- Optimization of Onshore Oil and Gas points
- Design of a new Coastal Fleet for supplying cargos for small port-points
- Find optimal ship for replacement the old vessel

Notes:

- Ships prototypes characteristics should be carefully analized before inclusion into FleetOptimizer for every specific Fleet and Task. 
- Data available in FleetOptimizer app is only for demo purpose.
- Ship Design modules of the application could require revision and extension. New Design modules could be added for specific types of Ships.  
  

Fleet Optimization tasks has long history.
Fleet Optimizer is a legacy project, developed in 2002-2005 using VC++6. Current application version was changed to work with Visual Studio 2019. 
This software is using Microsoft MFC technology for Graphic User interface.  Project includes 3 modules:  

- FleetOptimizer
- MMI
- ShipDesign

FleetOptimizer is a desktop Windows application.
New Cloud version is in development now. It will be based on Microservices architecture, will be multiplatform, and will work in Kubernetes on any of available Cloud platforms as Azure, AWS, GCP.
But suppose that we will continue to work also on this legacy desktop application - FleetOptimizer.

Ideas from following Fundamental ShipDesign Theory Works were used in FleetOptimizer's ShipDesign module.

1. Demeshko G. F. Proektirovanie sudov: Amfibiynye suda na vozdushnoy podushke. V.1, V.2. SPb.: Sudostroenie, 1992
2. Ashik V.V. Proektirovanie sudov. Leningrad: Shipbuilding, 1985
3. Nogid L.M. Proektirovanie morskih sudov. Leningrad: Shipbuilding, 1976
