#!/usr/bin/env Rscript

require(dplyr)
require(data.table)
require(compiler)

invisible(enableJIT(3))

heur <- fread("results.csv") %>% dplyr::select("instance","seed","cost","dist","tard","tmax","time")
lb <- fread("results-exactMip.csv") %>% dplyr::select("instance", "obj") %>% dplyr::rename(lb.value=obj)
unused <- fread("unused.csv")
metad <- fread("metad.csv")

cat("Heuristic results contains", nrow(heur), "rows.\n")
cat("LP lower bound results contains", nrow(lb), "rows.\n")
cat("Counting of unused vehicles contains", nrow(unused), "rows.\n")
cat("\n\n")

j1 <- dplyr::inner_join(heur, lb, by="instance", suffix=c("",""))
j2 <- dplyr::inner_join(j1, unused, by="instance", suffix=c("",""))
j3 <- dplyr::inner_join(j2, metad, by="instance", suffix=c("", ""))

j3 <- j3 %>% dplyr::mutate(
   gap=(cost-lb.value)/lb.value * 100.0
) %>% dplyr::arrange(-gap) %>% dplyr::select(
   instance,seed,patients,caregivers,depot,distribution,dens,lb.value,cost,gap,
   unused.vehi,dist,tard,tmax
)

fwrite(j3, "merged.csv")

