
<script lang="ts">
interface Tag {
    uuid: String;
    name: String;
}

  export let tags: Tag[] = []; 
</script>

<div class="tag-container">
  {#each tags as tag}
    <span class="rounded-xl bg-skyblue p-1">{tag.name}</span>
  {/each}
</div>
